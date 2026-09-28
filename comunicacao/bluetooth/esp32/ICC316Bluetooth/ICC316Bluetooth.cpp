#include "ICC316Bluetooth.h"

#include <NimBLEDevice.h>
#include <ArduinoJson.h>
#include "config.h"

namespace {
NimBLEServer* server = nullptr;
NimBLECharacteristic* measurementCharacteristic = nullptr;
NimBLECharacteristic* ackCharacteristic = nullptr;

volatile bool centralConnected = false;
volatile bool ackReceived = false;
bool initialized = false;

class ServerCallbacks : public NimBLEServerCallbacks {
public:
    void onConnect(NimBLEServer*, NimBLEConnInfo&) override {
        centralConnected = true;
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo&, int) override {
        centralConnected = false;
        pServer->startAdvertising();
    }
};

class AckCallbacks : public NimBLECharacteristicCallbacks {
public:
    void onWrite(NimBLECharacteristic* characteristic,
                 NimBLEConnInfo&) override {
        if (characteristic->getValue() == ICC316_BT_ACK) {
            ackReceived = true;
        }
    }
};

void initializeBluetooth() {
    if (initialized) return;

    NimBLEDevice::init(BLUETOOTH_DEVICE_NAME);

    server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    NimBLEService* service =
        server->createService(BLUETOOTH_SERVICE_UUID);

    measurementCharacteristic =
        service->createCharacteristic(
            BLUETOOTH_MEASUREMENT_UUID,
            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
        );

    ackCharacteristic =
        service->createCharacteristic(
            BLUETOOTH_ACK_UUID,
            NIMBLE_PROPERTY::WRITE
        );

    ackCharacteristic->setCallbacks(new AckCallbacks());

    service->start();

    NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
    advertising->setName(BLUETOOTH_DEVICE_NAME);
    advertising->addServiceUUID(BLUETOOTH_SERVICE_UUID);
    advertising->start();

    initialized = true;
}

bool waitForConnection() {
    const unsigned long start = millis();

    while (!centralConnected) {
        if (millis() - start >= BLUETOOTH_CONNECTION_TIMEOUT_MS)
            return false;
        delay(50);
    }
    return true;
}
}

bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
) {
    initializeBluetooth();

    if (!centralConnected && !waitForConnection())
        return false;

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

    ackReceived = false;
    measurementCharacteristic->setValue(payload.c_str());
    measurementCharacteristic->notify();

    const unsigned long start = millis();

    while (!ackReceived) {
        if (!centralConnected) return false;
        if (millis() - start >= BLUETOOTH_ACK_TIMEOUT_MS)
            return false;
        delay(20);
    }

    return true;
}
