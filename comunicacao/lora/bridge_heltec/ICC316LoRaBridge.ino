#include <RadioLib.h>

SX1262 radio = new Module(8, 14, 12, 13);

// Estes parâmetros devem ser iguais aos utilizados pelo nó sensor.
#define LORA_FREQUENCY_MHZ 915.0
#define LORA_BANDWIDTH_KHZ 125.0
#define LORA_SPREADING_FACTOR 7
// RadioLib: 5 = coding rate 4/5.
#define LORA_CODING_RATE 5
#define LORA_SYNC_WORD 0x12
#define LORA_OUTPUT_POWER_DBM 14
#define LORA_PREAMBLE_LENGTH 8
#define LORA_TCXO_VOLTAGE 1.8

#define SERIAL_BAUDRATE 115200
#define LORA_RX_TIMEOUT_MS 30000

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    delay(1000);

    // A ponte usa a mesma configuração LoRa do nó sensor.
    int16_t state = radio.begin(
        LORA_FREQUENCY_MHZ, LORA_BANDWIDTH_KHZ,
        LORA_SPREADING_FACTOR, LORA_CODING_RATE,
        LORA_SYNC_WORD, LORA_OUTPUT_POWER_DBM,
        LORA_PREAMBLE_LENGTH, LORA_TCXO_VOLTAGE
    );

    if (state != RADIOLIB_ERR_NONE) {
        Serial.printf("RADIO_ERROR:%d\n", state);
        while (true) delay(1000);
    }

    Serial.println("BRIDGE_READY");
}

void loop() {
    String payload;
    int16_t state = radio.receive(payload, LORA_RX_TIMEOUT_MS);

    if (state != RADIOLIB_ERR_NONE) {
        if (state != RADIOLIB_ERR_RX_TIMEOUT)
            Serial.printf("RADIO_ERROR:%d\n", state);
        return;
    }

    Serial.print("DATA:");
    Serial.println(payload);

    unsigned long start = millis();

    while (millis() - start < 5000) {
        if (Serial.available()) {
            String command = Serial.readStringUntil('\n');
            command.trim();

            if (command == "OK") {
                radio.transmit("OK");
                return;
            }

            if (command == "ERROR") {
                return;
            }
        }
        delay(10);
    }
}
