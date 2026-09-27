#ifndef ICC316_WIFI_H
#define ICC316_WIFI_H

#include <Arduino.h>

/*
 * Envia uma medição para o gateway Raspberry Pi.
 *
 * Retorna:
 *   true  -> medição recebida e confirmada pelo gateway
 *   false -> falha na comunicação ou resposta inválida
 *
 * Parâmetros:
 *   timestamp   - timestamp da medição
 *   temperature - temperatura da água (°C)
 *   ph          - pH; usar NAN se indisponível
 *   turbidity   - turbidez; usar NAN se indisponível
 */
bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
);

#endif