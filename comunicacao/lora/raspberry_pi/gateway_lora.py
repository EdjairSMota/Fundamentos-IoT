from pathlib import Path
import csv
import json
import math
import serial

from config import SERIAL_PORT, SERIAL_BAUDRATE

CSV_FILE = Path(__file__).resolve().parent / "gateway.csv"
CSV_FIELDS = ["node_id", "timestamp", "temperature", "ph", "turbidity"]


def ensure_csv():
    if not CSV_FILE.exists():
        with CSV_FILE.open("w", newline="", encoding="utf-8") as f:
            csv.DictWriter(f, fieldnames=CSV_FIELDS).writeheader()


def valid_number(x):
    return isinstance(x, (int, float)) and not isinstance(x, bool) and math.isfinite(x)


def validate(data):
    if not isinstance(data, dict):
        return False
    for field in CSV_FIELDS:
        if field not in data:
            return False
    if not isinstance(data["node_id"], str):
        return False
    if not isinstance(data["timestamp"], str):
        return False
    if not valid_number(data["temperature"]):
        return False
    for field in ("ph", "turbidity"):
        if data[field] is not None and not valid_number(data[field]):
            return False
    return True


def store(data):
    row = dict(data)
    if row["ph"] is None: row["ph"] = "NA"
    if row["turbidity"] is None: row["turbidity"] = "NA"
    with CSV_FILE.open("a", newline="", encoding="utf-8") as f:
        csv.DictWriter(f, fieldnames=CSV_FIELDS).writerow(row)


def main():
    ensure_csv()
    print("Gateway LoRa iniciado.")

    with serial.Serial(SERIAL_PORT, SERIAL_BAUDRATE, timeout=1) as port:
        while True:
            line = port.readline().decode("utf-8", errors="replace").strip()
            if not line.startswith("DATA:"):
                continue

            try:
                data = json.loads(line[5:])
            except json.JSONDecodeError:
                port.write(b"ERROR\n")
                continue

            if not validate(data):
                print("Medição rejeitada.")
                port.write(b"ERROR\n")
                continue

            store(data)
            print(f"Recebido e armazenado: {data['node_id']} {data['timestamp']}")
            port.write(b"OK\n")


if __name__ == "__main__":
    main()
