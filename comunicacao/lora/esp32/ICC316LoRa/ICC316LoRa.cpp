#include "ICC316LoRa.h"

#include <RadioLib.h>
#include <ArduinoJson.h>
#include "config.h"

namespace {
SX1262 radio = new Module(
    LORA_CS_PIN, LORA_DIO1_PIN, LORA_RST_PIN, LORA_BUSY_PIN
);
bool initialized = false;

bool initializeRadio() {
    if (initialized) return true;

    int16_t state = radio.begin(
        LORA_FREQUENCY_MHZ,
        LORA_BANDWIDTH_KHZ,
        LORA_SPREADING_FACTOR,
        LORA_CODING_RATE,
        LORA_SYNC_WORD,
        LORA_OUTPUT_POWER_DBM,
        LORA_PREAMBLE_LENGTH,
        LORA_TCXO_VOLTAGE
    );

    if (state != RADIOLIB_ERR_NONE) return false;
    initialized = true;
    return true;
}
}

bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
) {
    if (!initializeRadio()) return false;

    JsonDocument doc;
    doc["node_id"] = NODE_ID;
    doc["timestamp"] = timestamp;
    doc["temperature"] = temperature;

    if (isnan(ph)) doc["ph"] = nullptr;
    else doc["ph"] = ph;

    if (isnan(turbidity)) doc["turbidity"] = nullptr;
    else doc["turbidity"] = turbidity;

    String payload;
    serializeJson(doc, payload);

    int16_t state = radio.transmit(payload);
    if (state != RADIOLIB_ERR_NONE) return false;

    String ack;
    state = radio.receive(ack, LORA_ACK_TIMEOUT_MS);
    if (state != RADIOLIB_ERR_NONE) return false;

    return ack == LORA_ACK_OK;
}
