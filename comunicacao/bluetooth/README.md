# Comunicação Bluetooth — ICC316 Fundamentos de IoT

Este módulo fornece a comunicação Bluetooth Low Energy (BLE) para a atividade
prática de comunicação wireless da disciplina ICC316 — Fundamentos de IoT.

## Interface com o código do grupo

O código desenvolvido pelo grupo deve utilizar somente a função:

```cpp
bool sucesso = sendMeasurement(
    timestamp,
    temperature,
    ph,
    turbidity
);
```

A biblioteca fornecida pela disciplina é responsável pela comunicação BLE.
O grupo não precisa implementar NimBLE, GATT, descoberta de dispositivos ou
notificações.

## Arquitetura

```text
Heltec V3 / ESP32-S3
        |
        | BLE
        v
Raspberry Pi
        |
        v
gateway.csv
```

O ESP32 atua como periférico BLE. O Raspberry Pi atua como central/cliente BLE.

## Configuração do ESP32

Na pasta `ICC316Bluetooth`, copie:

```text
config.example.h -> config.h
```

e ajuste apenas o que for solicitado pelo professor, especialmente `NODE_ID`
e `BLUETOOTH_DEVICE_NAME`.

O arquivo `config.h` não deve ser enviado ao GitHub, pois pode conter a
configuração específica do grupo.

## Configuração do Raspberry Pi

Na pasta `raspberry_pi`, copie:

```text
config.example.py -> config.py
```

A configuração do adaptador Bluetooth do Raspberry Pi fica exclusivamente em
`config.py`.

Para verificar o adaptador disponível:

```bash
bluetoothctl list
```

Normalmente será utilizado `hci0`.

## Dependências

No Raspberry Pi:

```bash
sudo apt update
sudo apt install bluez python3-bleak
```

## Execução

No Raspberry Pi:

```bash
cd raspberry_pi
python3 gateway_bluetooth.py
```

O gateway procura nós cujo nome começa com `DEVICE_NAME_PREFIX`, conecta-se
ao nó, recebe a medição, grava o registro em `gateway.csv` e envia `OK`.

## Teste

O teste deve ser realizado com o hardware disponível. Verifique:

1. descoberta do nó BLE;
2. conexão entre Raspberry Pi e ESP32;
3. recebimento da notificação;
4. gravação da medição no CSV;
5. envio do ACK `OK`;
6. retorno `true` de `sendMeasurement()`.

Os detalhes internos de NimBLE e Bleak são fornecidos pela disciplina e não
fazem parte da implementação exigida do grupo.
