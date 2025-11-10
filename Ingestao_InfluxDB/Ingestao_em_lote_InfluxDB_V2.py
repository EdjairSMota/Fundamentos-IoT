# Este script assume que a filtragem gerou um arquivo chamado datalog_clean.csv 
# Ele usa o Token de API e a biblioteca influxdb-client para enviar todos os dados 
# em um lote eficiente para o InfluxDB.

import csv
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime

# Configurações de acesso ao InfluxDB
TOKEN = "SEU_TOKEN_DE_API_DO_INFLUXDB" # O Token gerado pelo sistema (veja o slide 20)
ORG = "NOME_SUA_ORGANIZACAO" # Pode ser o nome do grupo
BUCKET = "dados_agua" # O Bucket que você criou
URL = "http://localhost:8086" # Endereço do InfluxDB no Gateway

# Configurações do arquivo de entrada
INPUT_FILE = 'datalog_clean.csv'
MEASUREMENT = "temperatura" # Nome da medição no InfluxDB

def ingest_csv_to_influx(file_path):
    """
    Lê o arquivo limp ("Ingestao_em_lote_InfluxDB_V2.py"), converte cada linha para Line Protocol (ponto) e envia para o InfluxDB.
    """
    points = []

    try:
        # Inicializa o cliente e a API de escrita
        client = InfluxDBClient(url=URL, token=TOKEN, org=ORG)
        write_api = client.write_api(write_options=SYNCHRONOUS)

        with open(file_path, 'r') as csvfile:
            reader = csv.reader(csvfile)
            header = next(reader) # Pula o cabeçalho
            
            print(f"Iniciando ingestão de dados em lote no bucket '{BUCKET}'...")

            for row in reader:
                try:
                    # Assumindo o formato do CSV limpo: [Timestamp_MS, Temp_DS, Temp_NTC]
                    timestamp_ms = int(row[0])
                    temp_ds = float(row[1])
                    temp_ntc = float(row[2])
                    
                    # Converte o timestamp de milissegundos para nanossegundos (padrão InfluxDB V2)
                    timestamp_ns = timestamp_ms * 1_000_000

                    # 1. Ponto de dado para o Sensor DS18B20
                    point_ds = (
                        Point(MEASUREMENT)
                        .tag("sensor", "ds18b20")
                        .tag("localizacao", "tanque_teste")
                        .field("temp_c", temp_ds)
                        .time(timestamp_ns, WritePrecision.NS)
                    )
                    points.append(point_ds)

                    # 2. Ponto de dado para o Sensor NTC10K
                    point_ntc = (
                        Point(MEASUREMENT)
                        .tag("sensor", "ntc10k")
                        .tag("localizacao", "tanque_teste")
                        .field("temp_c", temp_ntc)
                        .time(timestamp_ns, WritePrecision.NS)
                    )
                    points.append(point_ntc)
                    
                except Exception as e:
                    print(f"Aviso: Linha pulada devido a erro de conversão ou formatação: {e}")
                    continue

        # Envio em lote: a biblioteca faz o envio otimizado de todos os pontos
        write_api.write(bucket=BUCKET, org=ORG, record=points)
        print("\nIngestão concluída. Todos os pontos foram enviados para o InfluxDB.")
        
    except FileNotFoundError:
        print(f"Erro: Arquivo de entrada '{file_path}' não encontrado. Executar o script de limpeza primeiro.")
    except Exception as e:
        print(f"Erro grave durante a ingestão: {e}")

if __name__ == '__main__':
    ingest_csv_to_influx(INPUT_FILE)