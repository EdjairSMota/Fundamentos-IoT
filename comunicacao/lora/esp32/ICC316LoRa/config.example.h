#ifndef ICC316_LORA_CONFIG_H
#define ICC316_LORA_CONFIG_H

#define NODE_ID "grupo_01"

#define LORA_CS_PIN 8
#define LORA_DIO1_PIN 14
#define LORA_RST_PIN 12
#define LORA_BUSY_PIN 13

#define LORA_FREQUENCY_MHZ 915.0
#define LORA_BANDWIDTH_KHZ 125.0
#define LORA_SPREADING_FACTOR 7

// RadioLib usa 5 para representar coding rate 4/5.
#define LORA_CODING_RATE 5

#define LORA_SYNC_WORD 0x12
#define LORA_OUTPUT_POWER_DBM 14
#define LORA_PREAMBLE_LENGTH 8

// TCXO da Heltec V3.
#define LORA_TCXO_VOLTAGE 1.8

#define LORA_ACK_TIMEOUT_MS 5000
#define LORA_ACK_OK "OK"

#endif
