import streamlit as st
import redis
import json
import time
import plotly.graph_objects as go

st.set_page_config(page_title="VM Monitor", layout="wide")
st.title("VM Resource Monitor")

# Conectar ao Redis
@st.cache_resource
def get_redis():
    return redis.Redis(host='192.168.121.171', port=6379, decode_responses=True)

placeholder = st.empty()

while True:
    try:
        r = get_redis()
        data = r.get('2020006973-proj3-output')
        
        if data:
            metrics = json.loads(data)
            
            with placeholder.container():
                # Última atualização
                st.write(f"**Última atualização:** {metrics.get('timestamp', 'N/A')}")
                
                # Métricas principais
                col1, col2 = st.columns(2)
                
                col1.metric("Tráfego Saída (%)", 
                           f"{metrics.get('percent-network-egress', 0):.2f}")
                
                col2.metric("Memória Cache (%)", 
                           f"{metrics.get('percent-memory-cache', 0):.2f}")
        
                st.divider()
                
                # Gráfico de CPU
                st.subheader("CPU Utilization (60s)")
                cpu_data = {}
                for key, value in metrics.items():
                    if key.startswith('avg-util-cpu'):
                        cpu_id = key.split('cpu')[1].split('-')[0]
                        cpu_data[f'CPU {cpu_id}'] = value
                
                if cpu_data:
                    fig = go.Figure(data=[
                        go.Bar(x=list(cpu_data.keys()), 
                               y=list(cpu_data.values()),
                               marker_color='lightblue')
                    ])
                    fig.update_layout(height=300, showlegend=False)
                    st.plotly_chart(fig, use_container_width=True)

                               
                # Dados 
                st.divider()
                with st.expander("Raw JSON"):
                    st.json(metrics)
        else:
            st.warning("Aguardando dados...")
    
    except Exception as e:
        st.error(f"Erro: {e}")
    
    time.sleep(5)
