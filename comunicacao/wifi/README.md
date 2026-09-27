# Comunicação Wi-Fi — ICC316 Fundamentos de IoT

Esta pasta contém o código fornecido para a atividade prática de comunicação wireless da disciplina ICC316 — Fundamentos de IoT.

## Objetivo

Permitir que o nó sensor baseado em ESP32 envie medições para um Raspberry Pi atuando como gateway.

A implementação da comunicação Wi-Fi é fornecida pela disciplina. Os grupos não precisam implementar o protocolo de comunicação.

## Interface com o código do grupo

O código desenvolvido pelo grupo deve utilizar a função:

```cpp
bool sucesso = sendMeasurement(
    timestamp,
    temperature,
    ph,
    turbidity
);
```

A função recebe os dados de uma medição e tenta enviá-los ao gateway.

O retorno da função indica se o gateway confirmou o recebimento da medição:

- `true` — a medição foi recebida e confirmada pelo gateway;
- `false` — a medição não foi enviada com sucesso ou não houve confirmação do gateway.

## Estrutura

A implementação está dividida em duas partes:

```text
wifi/
├── esp32/
│   └── ICC316WiFi/
│       ├── ICC316WiFi.h
│       ├── ICC316WiFi.cpp
│       └── config.example.h
│
└── raspberry_pi/
    └── gateway_wifi.py
```

### ESP32

A biblioteca `ICC316WiFi` implementa a comunicação Wi-Fi entre o nó sensor e o gateway.

O código do grupo deve utilizar a função `sendMeasurement()` para solicitar o envio de cada medição.

### Raspberry Pi

O programa `gateway_wifi.py` executa no Raspberry Pi e atua como gateway.

Ele recebe as medições enviadas pelo ESP32 e as armazena em um arquivo CSV.

## Configuração do ESP32

O arquivo `config.example.h` apresenta um modelo de configuração.

Cada grupo deverá criar, em seu computador, um arquivo chamado:

```text
config.h
```

a partir desse modelo.

O arquivo `config.h` deverá conter as informações específicas utilizadas durante o experimento, como:

- nome da rede Wi-Fi;
- senha da rede Wi-Fi;
- endereço IP do Raspberry Pi;
- porta utilizada pelo gateway;
- identificação do nó.

O arquivo `config.h` é uma configuração local e **não deve ser enviado ao GitHub**.

## Formato da mensagem

A biblioteca envia para o gateway uma mensagem no formato JSON.

Exemplo:

```json
{
    "node_id": "grupo_01",
    "timestamp": "2026-09-25T14:35:00",
    "temperature": 27.4,
    "ph": 6.8,
    "turbidity": 12.3
}
```

Quando um sensor não estiver disponível, o valor correspondente poderá ser enviado como `null`.

Exemplo:

```json
{
    "node_id": "grupo_01",
    "timestamp": "2026-09-25T14:35:00",
    "temperature": 27.4,
    "ph": null,
    "turbidity": 12.3
}
```

## Gateway

O Raspberry Pi executa o programa:

```text
gateway_wifi.py
```

O programa recebe as mensagens enviadas pelo ESP32 através do endpoint:

```text
POST /dados
```

Após receber e validar uma medição, o gateway registra os dados no arquivo:

```text
gateway.csv
```

O arquivo é criado automaticamente quando necessário.

## Dados armazenados

O arquivo CSV possui as seguintes colunas:

```text
node_id,timestamp,temperature,ph,turbidity
```

Exemplo:

```text
grupo_01,2026-09-25T14:35:00,27.4,6.8,12.3
```

Quando uma medição não estiver disponível, o gateway registra `NA` no campo correspondente.

O arquivo `gateway.csv` contém os dados produzidos durante o experimento e não faz parte do código fornecido pela disciplina.