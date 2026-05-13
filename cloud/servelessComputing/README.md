# ⚡ Serverless VM Monitor — Function Runtime + Live Dashboard

A serverless computing pipeline built from scratch: a custom AWS Lambda-inspired runtime, a real-time metrics processing function, and a live monitoring dashboard — all running on Kubernetes.

---

## Overview

This project covers three layers of the serverless stack:

1. **Serverless function** that processes VM resource metrics (CPU, memory, network) and computes derived stats like moving averages
2. **Live monitoring dashboard** that reads processed metrics from Redis and displays them in real time
3. **Custom serverless runtime** built from scratch — a drop-in replacement for the provided runtime, with extra features like Zip-based multi-file function support and configurable entry points

**Stack:** Python · Redis · Kubernetes · ConfigMaps · Docker · Plotly Dash / Streamlit · psutil

---

## Architecture

```
┌──────────────────────┐      every 5s       ┌──────────────────────┐
│   psutil collector   │ ──── metrics ──────► │       Redis          │
│   (provided, VM)     │                      │  key: "metrics"      │
└──────────────────────┘                      └──────────┬───────────┘
                                                         │ read input
                                              ┌──────────▼───────────┐
                                              │  Serverless Runtime  │
                                              │  (container/k8s pod) │
                                              │                      │
                                              │  calls handler(      │
                                              │    input, context)   │
                                              └──────────┬───────────┘
                                                         │ write output
                                              ┌──────────▼───────────┐
                                              │       Redis          │
                                              │  key: "<id>-proj3-   │
                                              │         output"      │
                                              └──────────┬───────────┘
                                                         │ read + render
                                              ┌──────────▼───────────┐
                                              │  Monitoring Dashboard│
                                              │  (k8s pod, web UI)   │
                                              └──────────────────────┘
```

---

## Task 1 — Serverless Function

The function lives in a single Python module and follows the Lambda-style interface:

```python
def handler(input: dict, context: object) -> dict[str, Any]:
    ...
```

### What it computes

Given a raw psutil snapshot every 5 seconds, the function outputs:

- **`percent-network-egress`** — outgoing bytes as a fraction of total network traffic
- **`percent-memory-cache`** — proportion of memory used for buffers + cache
- **`avg-util-cpuN-60sec`** — 1-minute moving average utilization per CPU core

### Moving average approach

Since serverless functions are stateless by definition, the moving average is maintained using the `context.env` dict — a small persistent dictionary that the runtime preserves between calls. Each invocation loads the last stored samples from `context.env`, appends the new measurement, trims to a 60-second window, and recalculates the average.

```python
# simplified
window = context.env.get("cpu0_window", [])
window.append(input["cpu_percent-0"])
window = window[-12:]  # 12 samples × 5s = 60s
context.env["cpu0_window"] = window
return {"avg-util-cpu0-60sec": sum(window) / len(window), ...}
```

### Deployment via ConfigMaps

The function is injected into the runtime pod as a Kubernetes ConfigMap — no custom image build needed:

```bash
# Mount the function code
kubectl create configmap pyfile --from-file pyfile=handler.py --output yaml > configmap-pyfile.yaml

# Set the Redis output key
kubectl create configmap outputkey \
  --from-literal REDIS_OUTPUT_KEY=<student-id>-proj3-output \
  --output yaml > configmap-outputkey.yaml

kubectl apply -f configmap-pyfile.yaml
kubectl apply -f configmap-outputkey.yaml
kubectl apply -f deployment.yaml
```

---

## Task 2 — Monitoring Dashboard

A live dashboard reads the processed metrics from Redis and renders time-series charts for CPU, memory, and network usage.

### Features

- Auto-refreshes every 5 seconds (matches the data collection interval)
- CPU utilization per core — individual line charts with 1-min moving average overlay
- Memory breakdown — stacked view of used / cached / buffer / free
- Network traffic — bytes sent vs. received, with egress percentage gauge

### Stack

Built with **Plotly Dash** (Python), reading from Redis via `redis-py`. Packaged in Docker and deployed as a Kubernetes Service.

```bash
# Test locally first
docker build -t vm-dashboard .
docker run --publish 30602:8050 vm-dashboard

# Then deploy to k8s
kubectl apply -f dashboard-deployment.yaml
kubectl apply -f dashboard-service.yaml
```

### SSH tunnel to access in browser

Since the cluster isn't publicly routable:

```bash
ssh -fNT -L 8080:<cluster-ip>:30602 <username>@vcm
# Then open http://localhost:8080
```

---

## Task 3 — Custom Serverless Runtime

A from-scratch Python runtime that replaces the provided `lucasmsp/serverless:redis` image. Fully compatible with any function built for this project.

### What it does

Runs in a loop, polling Redis for new metric snapshots. When new data arrives (detected by comparing the timestamp or a hash of the payload), it:

1. Loads the user's function module (from ConfigMap or Zip)
2. Builds the `context` object with all required fields + persistent `env`
3. Calls `handler(input, context)`
4. Serializes the result as JSON and writes it to the output Redis key
5. Persists `context.env` changes for the next invocation

### Extensions over the base runtime

| Feature | Base runtime | This runtime |
|---|---|---|
| Configurable Redis input key | ❌ hardcoded | ✅ `REDIS_INPUT_KEY` ConfigMap |
| Configurable poll interval | ❌ fixed 5s | ✅ `REDIS_POLL_PERIOD` ConfigMap |
| Multi-file function (Zip) | ❌ | ✅ `zipfile` ConfigMap (path or base64) |
| Custom entry point | ❌ always `handler` | ✅ `FUNCTION_HANDLER` ConfigMap |
| Pre-installed packages | minimal | numpy, pandas, sklearn, requests, matplotlib, tqdm |

### Zip-based function loading

For functions spanning multiple files, the runtime accepts a ConfigMap with a URL to a `.zip` archive:

```bash
kubectl create configmap zipfile \
  --from-literal FUNCTION_ZIP_URL=https://<host>/myfunction.zip
```

The runtime downloads, extracts, and adds the directory to `sys.path` before importing the module. The entry point is whatever function is specified in `FUNCTION_HANDLER` (defaults to `handler`).

### Context object

```python
class Context:
    host: str               # Redis hostname
    port: int               # Redis port
    input_key: str          # Redis key being monitored
    output_key: str         # Redis key for output
    function_getmtime: float  # Last modification time of the function file
    last_execution: float   # Timestamp of last successful execution
    env: dict               # Persisted state across invocations (mutable)
```

### Running the custom runtime

```bash
docker build -t custom-serverless-runtime .
# Apply the modified deployment (points to your image instead of lucasmsp/serverless:redis)
kubectl apply -f runtime-deployment.yaml
```

---

## Project Structure

```
├── task1/
│   ├── handler.py                  # Serverless function (handler + moving avg logic)
│   ├── configmap-pyfile.yaml
│   └── configmap-outputkey.yaml
├── task2/
│   ├── dashboard/
│   │   ├── Dockerfile
│   │   ├── requirements.txt
│   │   └── app.py                  # Plotly Dash app reading from Redis
│   ├── dashboard-deployment.yaml
│   └── dashboard-service.yaml
└── task3/
    ├── runtime/
    │   ├── Dockerfile
    │   ├── requirements.txt
    │   └── runtime.py              # Custom serverless runtime loop
    └── runtime-deployment.yaml     # Extended deployment with new ConfigMap options
```

---

## Redis Quick Reference

```bash
# Check what's in Redis
redis-cli keys "*"
redis-cli get metrics              # raw VM snapshot (JSON)
redis-cli get <id>-proj3-output    # processed function output (JSON)

# Monitor in real time
redis-cli monitor
```

---

## Context

Built as part of a graduate course on **Cloud Computing**, covering the serverless computing model, event-driven function execution, real-time data pipelines, and cloud-native observability patterns.
