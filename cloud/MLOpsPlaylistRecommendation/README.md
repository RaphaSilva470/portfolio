# 🎵 Playlist Recommender — MLOps on Kubernetes

A microservice-based playlist recommendation system built as a full MLOps pipeline: from Frequent Itemset Mining to containerized deployment with CI/CD automation via ArgoCD and Kubernetes.

---

## Overview

This project implements a music playlist recommendation service end-to-end, covering the full lifecycle from ML model training to cloud-native deployment. The system uses association rule mining to suggest playlists based on a user's listening history, and is deployed as a set of microservices orchestrated by Kubernetes with automated delivery through ArgoCD.

**Stack:** Python · Flask · Docker · Kubernetes · ArgoCD · FP-Growth / Apriori · REST API

---

## Architecture

The application is split into three independent components following microservice principles:

```
┌──────────────────┐     Shared PVC      ┌────────────────────┐
│   ML Container   │ ──── model.pkl ────► │  Frontend (Flask)  │
│  (rule gen job)  │                      │    REST API        │
└──────────────────┘                      └────────────────────┘
        ▲                                          ▲
        │  ArgoCD detects dataset update           │  User sends POST /api/recommend
        │  → triggers ML job rerun                 │
   Git Repo (YAML + dataset pointer + image tag)
```

### Components

**1. Playlist Rules Generator (ML Container)**
Runs as a Kubernetes Job. Loads the Spotify playlist dataset, applies Frequent Itemset Mining (FP-Growth or Apriori) to extract association rules, and persists the model to a shared PersistentVolume. Only reruns when the dataset or model configuration changes.

**2. REST API Server (Frontend Container)**
Flask app exposing `POST /api/recommend`. Loads the model from the shared volume on startup and watches for updates via file modification time or checksum (MD5/SHA). Always serves recommendations using the latest model version without downtime.

**3. REST API Client**
CLI client (via `wget`/`curl`) for sending recommendation requests. Accepts a list of songs and returns recommended playlists, along with the current model version and last update timestamp.

---

## ML Approach

The recommendation system is built on **Frequent Itemset Mining**:

- Each playlist is treated as a "basket" of songs
- FP-Growth extracts frequent co-occurring song sets above a configurable support threshold
- Association rules are derived in the form `if {Song A, Song B} → {Song C}` with a minimum confidence filter
- The generated rules are serialized with `pickle` and shared across containers via a Kubernetes PersistentVolume

Example rule output:
```python
[{'Yesterday', 'Let It Be'}, {'Hey Jude'}, confidence=0.82]
```

---

## API

**Endpoint:** `POST /api/recommend`

**Request:**
```json
{
  "songs": ["Bohemian Rhapsody", "Yesterday"]
}
```

**Response:**
```json
{
  "songs": ["Hey Jude", "Let It Be", "Come Together"],
  "version": "0.2",
  "model_date": "2024-03-15T10:42:00"
}
```

---

## CI/CD Pipeline

The deployment is fully automated through ArgoCD watching a Git repository:

| Trigger | What happens |
|---|---|
| Push new Docker image tag | ArgoCD detects change → Kubernetes pulls new image → rolling update |
| Update dataset pointer in YAML | ArgoCD triggers ML Job rerun → new model saved to PVC → Flask reloads |
| Change replica count in deployment YAML | ArgoCD syncs → Kubernetes scales pods automatically |

```bash
# Build and push a new version
docker build . -t quay.io/<user>/playlist-recommender:0.2
docker push quay.io/<user>/playlist-recommender:0.2
# Update image tag in deployment.yaml → push to Git → ArgoCD does the rest
```

---

## Project Structure

```
├── ml/
│   ├── Dockerfile
│   ├── requirements.txt
│   └── generate_rules.py       # FIM pipeline, saves model.pkl to shared volume
├── api/
│   ├── Dockerfile
│   ├── requirements.txt
│   └── app.py                  # Flask REST API, watches model for updates
├── client/
│   └── recommend.sh            # CLI wrapper around wget
├── k8s/
│   ├── deployment.yaml         # Frontend + ML Job specs
│   ├── service.yaml
│   └── pvc.yaml                # PersistentVolumeClaim for shared model storage
└── argocd/
    └── application.yaml        # ArgoCD app manifest
```

---

## Running Locally (Docker)

```bash
# 1. Train the model
docker build ./ml -t playlist-ml
docker run -v $(pwd)/model:/model playlist-ml

# 2. Start the API server
docker build ./api -t playlist-api
docker run --publish 5000:5000 -v $(pwd)/model:/model playlist-api

# 3. Test it
curl -X POST http://localhost:5000/api/recommend \
  -H 'Content-Type: application/json' \
  -d '{"songs": ["Bohemian Rhapsody", "Yesterday"]}'
```

---

## Deploying on Kubernetes

```bash
kubectl -n <namespace> apply -f k8s/pvc.yaml
kubectl -n <namespace> apply -f k8s/deployment.yaml
kubectl -n <namespace> apply -f k8s/service.yaml

# Check status
kubectl -n <namespace> get pods
kubectl -n <namespace> logs <pod-name>
```

---

## Dataset

Uses a sample of the [Spotify playlist dataset](https://www.aicrowd.com/challenges/spotify-million-playlist-dataset-challenge):

- `2023_spotify_ds1.csv` — ~240k playlists (initial training set)
- `2023_spotify_ds2.csv` — ~240k playlists (used to simulate a dataset update)
- `2023_spotify_songs.csv` — ~7k songs for generating recommendation requests

---

## Observations from CI/CD Testing

After running the full pipeline through ArgoCD, some findings worth noting:

- **Code update (new image tag):** Deployment update takes ~60–90s from Git push to new pod running. The service experiences minimal downtime during rolling update with more than one replica.
- **Dataset update:** Triggers the ML Job rerun (~2–5 min depending on dataset size), then the frontend reloads the model automatically. No API downtime since the old model stays loaded until the new one is detected.
- **Replica scaling:** Near-instant via ArgoCD sync — Kubernetes handles it without any service interruption.

Model change detection in the frontend is done by polling the model file's modification timestamp every N seconds. When a change is detected, the model is reloaded in-memory before the next request is served.

---

## Context

Built as part of a graduate course on **DevOps and Cloud Computing**, covering MLOps practices: containerization, orchestration, CI/CD automation, and persistent storage in a distributed environment.
