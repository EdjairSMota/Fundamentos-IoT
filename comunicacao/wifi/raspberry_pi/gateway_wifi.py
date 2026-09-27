from flask import Flask, request, jsonify
from pathlib import Path
import csv
import math


app = Flask(__name__)


# -------------------------------------------------------------
# Arquivo CSV
# -------------------------------------------------------------

CSV_FILE = Path(__file__).resolve().parent / "gateway.csv"

CSV_FIELDS = [
    "node_id",
    "timestamp",
    "temperature",
    "ph",
    "turbidity"
]


# -------------------------------------------------------------
# Cria o CSV caso ainda não exista
# -------------------------------------------------------------

def ensure_csv():

    if not CSV_FILE.exists():

        with CSV_FILE.open(
            "w",
            newline="",
            encoding="utf-8"
        ) as file:

            writer = csv.DictWriter(
                file,
                fieldnames=CSV_FIELDS
            )

            writer.writeheader()


# -------------------------------------------------------------
# Verifica se um valor é numérico finito
# -------------------------------------------------------------

def is_valid_number(value):

    return (
        isinstance(value, (int, float))
        and not isinstance(value, bool)
        and math.isfinite(value)
    )


# -------------------------------------------------------------
# Endpoint para receber medições
# -------------------------------------------------------------

@app.route("/dados", methods=["POST"])
def receive_data():

    data = request.get_json(silent=True)


    # ---------------------------------------------------------
    # JSON válido?
    # ---------------------------------------------------------

    if data is None:

        return jsonify({
            "status": "error",
            "message": "JSON invalido"
        }), 400


    # ---------------------------------------------------------
    # Campos obrigatórios
    # ---------------------------------------------------------

    required_fields = [
        "node_id",
        "timestamp",
        "temperature",
        "ph",
        "turbidity"
    ]


    for field in required_fields:

        if field not in data:

            return jsonify({
                "status": "error",
                "message": f"Campo ausente: {field}"
            }), 400


    # ---------------------------------------------------------
    # Validação dos tipos
    # ---------------------------------------------------------

    if not isinstance(data["node_id"], str):

        return jsonify({
            "status": "error",
            "message": "node_id invalido"
        }), 400


    if not isinstance(data["timestamp"], str):

        return jsonify({
            "status": "error",
            "message": "timestamp invalido"
        }), 400


    if not is_valid_number(data["temperature"]):

        return jsonify({
            "status": "error",
            "message": "temperature invalida"
        }), 400


    if (
        data["ph"] is not None
        and not is_valid_number(data["ph"])
    ):

        return jsonify({
            "status": "error",
            "message": "ph invalido"
        }), 400


    if (
        data["turbidity"] is not None
        and not is_valid_number(data["turbidity"])
    ):

        return jsonify({
            "status": "error",
            "message": "turbidity invalida"
        }), 400


    # ---------------------------------------------------------
    # Garantir que o CSV exista
    # ---------------------------------------------------------

    ensure_csv()


    # ---------------------------------------------------------
    # Preparar linha
    # ---------------------------------------------------------

    row = {
        "node_id": data["node_id"],
        "timestamp": data["timestamp"],
        "temperature": data["temperature"],
        "ph": (
            data["ph"]
            if data["ph"] is not None
            else "NA"
        ),
        "turbidity": (
            data["turbidity"]
            if data["turbidity"] is not None
            else "NA"
        )
    }


    # ---------------------------------------------------------
    # Gravar medição
    # ---------------------------------------------------------

    with CSV_FILE.open(
        "a",
        newline="",
        encoding="utf-8"
    ) as file:

        writer = csv.DictWriter(
            file,
            fieldnames=CSV_FIELDS
        )

        writer.writerow(row)


    # ---------------------------------------------------------
    # Confirmar recebimento
    # ---------------------------------------------------------

    return jsonify({
        "status": "ok"
    }), 200


# -------------------------------------------------------------
# Inicialização
# -------------------------------------------------------------

if __name__ == "__main__":

    ensure_csv()

    app.run(
        host="0.0.0.0",
        port=5000,
        debug=False
    )