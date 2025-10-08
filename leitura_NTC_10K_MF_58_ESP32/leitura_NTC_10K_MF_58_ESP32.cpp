// Incluir as bibliotecas
#include <OneWire.h>
#include <DallasTemperature.h>

// Declarar os pinos e os objetos para os sensores
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define NTC_PIN 34

// Iniciar os sensores
void setup() {
  sensors.begin();
}

// Chamar a função para ler a temperatura
void loop() {
  sensors.requestTemperatures();
  float tempDS18B20 = sensors.getTempCByIndex(0);
  int analogValueNTC = analogRead(NTC_PIN);
}

