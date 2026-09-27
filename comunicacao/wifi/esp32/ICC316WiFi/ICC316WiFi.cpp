#include "ICC316WiFi.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"


// -------------------------------------------------------------
// Garante que o ESP32 esteja conectado ao Wi-Fi
// -------------------------------------------------------------

bool ensureWiFiConnection()
{
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }

    WiFi.mode(WIFI_STA);

    WiFi.setAutoReconnect(true);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED) {

        if (millis() - startTime >= WIFI_TIMEOUT_MS) {
            return false;
        }

        delay(100);
    }

    return true;
}


// -------------------------------------------------------------
// Envia uma medição para o Raspberry Pi
// -------------------------------------------------------------

bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
) {

    // ---------------------------------------------------------
    // 1. Garantir conexão Wi-Fi
    // ---------------------------------------------------------

    if (!ensureWiFiConnection()) {
        return false;
    }


    // ---------------------------------------------------------
    // 2. Montar URL do gateway
    // ---------------------------------------------------------

    String url =
        String("http://") +
        GATEWAY_IP +
        ":" +
        String(GATEWAY_PORT) +
        "/dados";


    // ---------------------------------------------------------
    // 3. Criar objeto JSON
    // ---------------------------------------------------------

    JsonDocument doc;

    doc["node_id"] = NODE_ID;
    doc["timestamp"] = timestamp;
    doc["temperature"] = temperature;


    if (isnan(ph)) {
        doc["ph"] = nullptr;
    }
    else {
        doc["ph"] = ph;
    }


    if (isnan(turbidity)) {
        doc["turbidity"] = nullptr;
    }
    else {
        doc["turbidity"] = turbidity;
    }


    // ---------------------------------------------------------
    // 4. Serializar JSON
    // ---------------------------------------------------------

    String payload;

    serializeJson(
        doc,
        payload
    );


    // ---------------------------------------------------------
    // 5. HTTP POST
    // ---------------------------------------------------------

    HTTPClient http;

    http.setTimeout(HTTP_TIMEOUT_MS);

    if (!http.begin(url)) {
        return false;
    }

    http.addHeader(
        "Content-Type",
        "application/json"
    );

    int httpCode = http.POST(payload);


    // ---------------------------------------------------------
    // 6. Verificar resposta do gateway
    // ---------------------------------------------------------

    bool success = false;

    if (httpCode == HTTP_CODE_OK) {

        String response =
            http.getString();

        JsonDocument responseDoc;

        DeserializationError error =
            deserializeJson(
                responseDoc,
                response
            );

        if (!error) {

            const char* status =
                responseDoc["status"];

            if (
                status != nullptr &&
                String(status) == "ok"
            ) {
                success = true;
            }
        }
    }


    // ---------------------------------------------------------
    // 7. Encerrar conexão HTTP
    // ---------------------------------------------------------

    http.end();


    return success;
}