"""Slides 53–55 e 58: interpretar JSON e construir um Point."""
import json
from comum import construir_ponto

def processar_mensagem(mensagem):
    dados = json.loads(mensagem)
    if not isinstance(dados, dict):
        raise ValueError("A mensagem JSON deve ser um objeto.")
    return construir_ponto(dados)

if __name__ == "__main__":
    mensagem = ('{"node_id":"heltec_01","timestamp":"2026-10-18T12:00:00Z",'
                '"temperatura":27.4,"ph":6.8,"turbidez":12.3}')
    print(processar_mensagem(mensagem).to_line_protocol())
