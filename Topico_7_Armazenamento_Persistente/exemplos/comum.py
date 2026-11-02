"""Funções compartilhadas dos exemplos do Tópico 7."""
import os
from pathlib import Path
from datetime import datetime, timezone
from math import isfinite
from dotenv import load_dotenv
from influxdb_client import Point

load_dotenv(Path(__file__).resolve().parents[1] / ".env")

def config():
    nomes = ("INFLUXDB_URL", "INFLUXDB_TOKEN", "INFLUXDB_ORG", "INFLUXDB_BUCKET")
    faltando = [nome for nome in nomes if not os.getenv(nome)]
    if faltando:
        raise RuntimeError("Variáveis ausentes: " + ", ".join(faltando))
    return tuple(os.environ[nome] for nome in nomes)

def construir_ponto(dados):
    """Preserva o instante original e aceita apenas campos realmente disponíveis."""
    instante = datetime.fromisoformat(str(dados["timestamp"]).replace("Z", "+00:00"))
    if instante.tzinfo is None:
        raise ValueError("Timestamp precisa incluir fuso horário (Z ou offset).")
    ponto = (Point("qualidade_agua")
             .tag("node_id", str(dados["node_id"]))
             .tag("local", str(dados.get("local") or "lab_maker"))
             .time(instante.astimezone(timezone.utc)))
    encontrados = 0
    for campo in ("temperatura", "ph", "turbidez"):
        if dados.get(campo) not in (None, ""):
            valor = float(dados[campo])
            if not isfinite(valor):
                raise ValueError(f"Valor não finito em {campo}")
            ponto = ponto.field(campo, valor)
            encontrados += 1
    if not encontrados:
        raise ValueError("Nenhum campo de medição presente.")
    return ponto
