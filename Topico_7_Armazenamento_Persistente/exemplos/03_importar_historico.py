"""Slides 46–50: importar CSV preservando timestamps de coleta."""
import csv
import sys
from influxdb_client import InfluxDBClient
from influxdb_client.client.write_api import SYNCHRONOUS
from comum import config, construir_ponto

def main():
    if len(sys.argv) != 2:
        raise SystemExit("Uso: python exemplos/03_importar_historico.py dados/arquivo.csv")
    url, token, org, bucket = config()
    lidos = enviados = rejeitados = 0
    with InfluxDBClient(url=url, token=token, org=org) as cliente:
        with cliente.write_api(write_options=SYNCHRONOUS) as escrita:
            with open(sys.argv[1], encoding="utf-8-sig", newline="") as arquivo:
                for numero, dados in enumerate(csv.DictReader(arquivo), start=2):
                    lidos += 1
                    try:
                        escrita.write(bucket=bucket, record=construir_ponto(dados))
                        enviados += 1
                    except (ValueError, KeyError, TypeError) as erro:
                        rejeitados += 1
                        print(f"Linha {numero} rejeitada: {erro}")
    print(f"Lidos: {lidos}; enviados: {enviados}; rejeitados: {rejeitados}")
    print("Confira os registros no Data Explorer; envio não é prova de consulta.")

if __name__ == "__main__":
    main()
