import os
import sys
import json
import time
import importlib.util
import redis
import zipfile
import tempfile
import shutil
from pathlib import Path

class Context:
    def __init__(self, host, port, input_key, output_key):
        self.host = host
        self.port = port
        self.input_key = input_key
        self.output_key = output_key
        self.env = {}

def load_handler_from_file(pyfile_path, handler_name):
    spec = importlib.util.spec_from_file_location("usermodule", pyfile_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return getattr(module, handler_name)

def load_handler_from_zip(zip_path, handler_name, entry_file="handler.py"):
    temp_dir = tempfile.mkdtemp()
    
    try:
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(temp_dir)
        
        entry_path = os.path.join(temp_dir, entry_file)
        
        if not os.path.exists(entry_path):
            raise FileNotFoundError(f"{entry_file} não encontrado no ZIP")
        
        sys.path.insert(0, temp_dir)
        
        handler = load_handler_from_file(entry_path, handler_name)
        
        return handler, temp_dir
    
    except Exception as e:
        shutil.rmtree(temp_dir)
        raise e

def main():
    # Configurações
    host = os.getenv('REDIS_HOST', '192.168.121.171')
    port = int(os.getenv('REDIS_PORT', '6379'))
    input_key = os.getenv('REDIS_INPUT_KEY', 'metrics')
    output_key = os.getenv('REDIS_OUTPUT_KEY')
    period = int(os.getenv('REDIS_MONITORING_PERIOD', '5'))
    handler_name = os.getenv('REDIS_FUNCTION_HANDLER', 'handler')
    zip_path = os.getenv('REDIS_FUNCTION_ZIP_PATH', None)
    zip_entry = os.getenv('REDIS_FUNCTION_ZIP_ENTRY', 'handler.py')
    
    if not output_key:
        sys.exit(1)
    
    # Conectar Redis
    r = redis.Redis(host=host, port=port, decode_responses=True)
    try:
        r.ping()
    except Exception as e:
        sys.exit(1)
    temp_dir = None
    try:
        if zip_path and os.path.exists(zip_path):
            handler, temp_dir = load_handler_from_zip(zip_path, handler_name, zip_entry)
        else:
            handler = load_handler_from_file("/opt/usermodule.py", handler_name)
    except Exception as e:
        sys.exit(1)
    
    context = Context(host, port, input_key, output_key)
    last_data = None

    while True:
        try:
            data = r.get(input_key)
            
            if data is None:
                print("Data not available yet!")
            elif data != last_data:
                last_data = data
                input_dict = json.loads(data)
                result = handler(input_dict, context)
                r.set(output_key, json.dumps(result))
        except Exception as e:
            print(f"Erro: {e}")
            import traceback
            traceback.print_exc()
        
        time.sleep(period)

if __name__ == "__main__":
    main()

