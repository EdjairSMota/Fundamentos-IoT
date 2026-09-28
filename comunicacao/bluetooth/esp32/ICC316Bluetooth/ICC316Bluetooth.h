#ifndef ICC316_BLUETOOTH_H
#define ICC316_BLUETOOTH_H

#include <Arduino.h>

bool sendMeasurement(
    const String& timestamp,
    float temperature,
    float ph,
    float turbidity
);

#endif
