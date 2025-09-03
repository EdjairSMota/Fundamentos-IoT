#include <OneWire.h>
#include <DallasTemperature.h>

// Pino onde o sensor DS18B20 está conectado
#define ONE_WIRE_BUS 2

// Configura uma instância da biblioteca OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Passa a referência da instância OneWire para a biblioteca Dallas Temperature
DallasTemperature sensors(&oneWire);

void setup(void) {
  // Inicializa a comunicação serial para debug
  Serial.begin(9600);

  // Inicializa a biblioteca do sensor
  sensors.begin();
  Serial.println("Sensor DS18B20 inicializado!");
}

void loop(void) {
  // Solicita ao sensor que faça a leitura da temperatura
  sensors.requestTemperatures();

  // Lê a temperatura em Celsius do primeiro sensor encontrado
  float tempC = sensors.getTempCByIndex(0);

  // Lê a temperatura em Fahrenheit
  float tempF = sensors.getTempFByIndex(0);

  // Exibe a temperatura no Monitor Serial
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.print(" °C");
  Serial.print("  |  ");
  Serial.print(tempF);
  Serial.println(" °F");

  // Aguarda 2 segundos antes da próxima leitura
  delay(2000);
}