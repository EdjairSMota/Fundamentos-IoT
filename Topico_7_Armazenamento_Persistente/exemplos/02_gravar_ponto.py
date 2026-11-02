"""Slides 43–45: construir e gravar um ponto de demonstração."""
from datetime import datetime, timezone
from influxdb_client import InfluxDBClient
from influxdb_client.client.write_api import SYNCHRONOUS
from comum import config, construir_ponto

url, token, org, bucket = config()
dados = {
    "node_id": "heltec_exemplo",
    "timestamp": datetime.now(timezone.utc).isoformat(),
    "temperatura": 27.4,
    "ph": 6.8,
    "turbidez": 12.3,
}
with InfluxDBClient(url=url, token=token, org=org) as cliente:
    with cliente.write_api(write_options=SYNCHRONOUS) as escrita:
        escrita.write(bucket=bucket, record=construir_ponto(dados))
print("Escrita enviada. Confirme a persistência no Data Explorer.")
