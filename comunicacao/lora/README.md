# Comunicação LoRa — ICC316 Fundamentos de IoT

Este módulo fornece a comunicação LoRa para a atividade prática de
comunicação wireless da disciplina ICC316 — Fundamentos de IoT.

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

A biblioteca fornecida pela disciplina é responsável pela inicialização do
rádio, transmissão, espera do ACK e retorno de sucesso/falha.

## Arquitetura

```text
Heltec V3 — nó sensor
        |
        | LoRa
        v
Heltec V3 — ponte
        |
        | USB/Serial
        v
Raspberry Pi — gateway
        |
        v
gateway.csv
```

Não há um módulo LoRa dedicado conectado ao Raspberry Pi para esta atividade.
Por isso, a segunda Heltec V3 funciona como ponte entre LoRa e USB/Serial.

## Protocolo

A disciplina possui dois grupos. O protocolo foi mantido deliberadamente
simples: não serão acrescentados UUID de aplicação, números de sequência,
retransmissões ou mecanismos adicionais de endereçamento nesta atividade.

A própria medição contém `node_id` e `timestamp`.

Fluxo:

```text
ESP32 -> LoRa -> ponte -> USB -> Raspberry Pi
ESP32 <- LoRa <- ponte <- USB <- Raspberry Pi
```

O Raspberry Pi valida e grava a medição e envia `OK`. A ponte retransmite `OK`
por LoRa. Somente após receber esse ACK `sendMeasurement()` retorna `true`.

### Dois grupos compartilhando o canal

Os dois grupos utilizarão a mesma configuração inicial de rádio. Durante os
testes, os grupos devem evitar transmissões simultâneas para reduzir a
possibilidade de colisões no canal LoRa.

## Parâmetros LoRa

Configuração inicial utilizada nos dois lados:

```text
Frequência:       915 MHz
Largura de banda: 125 kHz
Spreading Factor: SF7
Coding Rate:      4/5
Sync Word:        0x12
Potência:         14 dBm
Preâmbulo:        8 símbolos
```

No RadioLib, `LORA_CODING_RATE 5` representa o coding rate 4/5. O valor `5`
não deve ser alterado para `4` apenas por causa da notação 4/5.

Os parâmetros da ponte devem permanecer iguais aos parâmetros do nó.

## Configuração do ESP32

Na pasta `ICC316LoRa`, copie:

```text
config.example.h -> config.h
```

e ajuste o `NODE_ID` conforme o grupo.

Os demais parâmetros LoRa devem permanecer iguais nos nós e na ponte,
salvo orientação do professor.

## Configuração do Raspberry Pi

Na pasta `raspberry_pi`, copie:

```text
config.example.py -> config.py
```

A porta serial da ponte deve ser configurada nesse arquivo.

Verifique as portas disponíveis, por exemplo:

```bash
ls /dev/ttyUSB*
ls /dev/ttyACM*
```

## Dependência do Raspberry Pi

Instale o suporte à porta serial:

```bash
sudo apt update
sudo apt install python3-serial
```

## Ponte Heltec

Carregue `ICC316LoRaBridge.ino` na Heltec V3 que será utilizada como ponte.

A ponte recebe a mensagem LoRa, envia a medição ao Raspberry Pi pela porta
USB/Serial e, quando recebe `OK` do gateway, transmite `OK` de volta ao nó.

## Execução do gateway

No Raspberry Pi:

```bash
cd raspberry_pi
python3 gateway_lora.py
```

## Teste

Os grupos devem testar no hardware:

1. inicialização do SX1262;
2. transmissão do nó;
3. recepção pela ponte;
4. comunicação USB/Serial entre ponte e Raspberry Pi;
5. gravação no CSV;
6. retorno do `OK` pelo caminho inverso;
7. retorno `true` de `sendMeasurement()`.

Caso ocorram dificuldades com o comportamento do rádio SX1262 entre
`transmit()` e `receive()`, o professor poderá contar com o auxílio de alunos
mais avançados do laboratório.
