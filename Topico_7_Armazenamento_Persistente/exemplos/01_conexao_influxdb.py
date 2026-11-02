"""Slides 31–33: verificar a saúde do serviço InfluxDB 2."""
from influxdb_client import InfluxDBClient
from comum import config

url, token, org, bucket = config()
with InfluxDBClient(url=url, token=token, org=org) as cliente:
    estado = cliente.health().status
    print("Estado do serviço:", estado)
    if estado != "pass":
        raise RuntimeError("InfluxDB não está saudável.")
print("A saúde do serviço não garante permissão de escrita no bucket.")
