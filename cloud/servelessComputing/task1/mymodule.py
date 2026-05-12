from typing import Any
import json

def handler(input: dict, context: object) -> dict[str, Any]:
    if not hasattr(context.env, '__dict__'):
        context.env = {}
    
    if 'cpu_history' not in context.env:
        context.env['cpu_history'] = {}
    
    if 'timestamps' not in context.env:
        context.env['timestamps'] = []
        
    # 1: Porcentagem de tráfego de saída
    bytes_sent = input.get('net_io_counters_eth0-bytes_sent1', 0)
    bytes_recv = input.get('net_io_counters_eth0-bytes_recv1', 0)
    
    total_bytes = bytes_sent + bytes_recv
    if total_bytes > 0:
        percent_egress = (bytes_sent / total_bytes) * 100
    else:
        percent_egress = 0.0
    
    # 2: Porcentagem de cache em memória 
    cached = input.get('virtual_memory-cached', 0)
    buffers = input.get('virtual_memory-buffers', 0)
    total_memory = input.get('virtual_memory-total', 1)
    
    percent_cache = ((cached + buffers) / total_memory) * 100 if total_memory > 0 else 0.0
    
    # 3: Média móvel de CPU 
    timestamp = input.get('timestamp', '')
    cpu_data = {}
    cpu_count = 0
    
    # Encontrar CPUs 
    for key, value in input.items():
        if key.startswith('cpu_percent-') and key != 'cpu_percent':
            cpu_id = key.replace('cpu_percent-', '')
            cpu_data[cpu_id] = float(value)
            cpu_count += 1
    
    for cpu_id, cpu_util in cpu_data.items():
        if cpu_id not in context.env['cpu_history']:
            context.env['cpu_history'][cpu_id] = []
        
        context.env['cpu_history'][cpu_id].append({
            'timestamp': timestamp,
            'utilization': cpu_util
        })
        
        if len(context.env['cpu_history'][cpu_id]) > 12:
            context.env['cpu_history'][cpu_id].pop(0)
    
    cpu_averages = {}
    for cpu_id, history in context.env['cpu_history'].items():
        if history:
            avg_util = sum(h['utilization'] for h in history) / len(history)
            cpu_averages[f'avg-util-cpu{cpu_id}-60sec'] = round(avg_util, 2)
    
    result = {
        'timestamp': timestamp,
        'percent-network-egress': round(percent_egress, 2),
        'percent-memory-cache': round(percent_cache, 2),
        **cpu_averages  
    }
    
    return result
