#ifndef ICC316_LORA_H
#define ICC316_LORA_H

#include <Arduino.h>

bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
);

#endif
