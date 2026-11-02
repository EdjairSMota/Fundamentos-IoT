"""Slides 56–59: estrutura para integrar o receptor do grupo.

TAREFA DOS ALUNOS: implementar receber_mensagem() reutilizando o
receptor Wi-Fi ou Bluetooth já desenvolvido na atividade anterior.
"""
import json
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "exemplos"))
from influxdb_client import InfluxDBClient
from influxdb_client.client.write_api import SYNCHRONOUS
from comum import config, construir_ponto

def receber_mensagem():
    """Retornar uma mensagem JSON produzida pelo receptor real do gateway."""
    raise NotImplementedError("Integrar aqui o receptor Wi-Fi ou Bluetooth do grupo.")

def main():
    url, token, org, bucket = config()
    with InfluxDBClient(url=url, token=token, org=org) as cliente:
        with cliente.write_api(write_options=SYNCHRONOUS) as escrita:
            while True:
                mensagem = receber_mensagem()
                try:
                    dados = json.loads(mensagem)
                    if not isinstance(dados, dict):
                        raise ValueError("Mensagem deve ser um objeto JSON.")
                    escrita.write(bucket=bucket, record=construir_ponto(dados))
                    print("Escrita enviada; verificar no Data Explorer.")
                except (ValueError, KeyError, TypeError) as erro:
                    print("Mensagem rejeitada:", erro)
                # Erros de conexão/escrita não são ocultados:
                # devem ser analisados no experimento de falhas.

if __name__ == "__main__":
    main()
