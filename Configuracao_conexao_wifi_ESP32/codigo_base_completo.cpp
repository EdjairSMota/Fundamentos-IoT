#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Configuração da Rede Wi-Fi
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

// Configuração do Servidor no Raspberry Pi
const char* serverAddress = "http://<IP_DO_RPI>:5000/datalogger";

// Configuração dos Pinos
const int chipSelect = 5; // Pino CS para o leitor de cartão microSD
#define ONE_WIRE_BUS 4    // Pino para o sensor DS18B20
#define NTC_PIN 34        // Pino para o sensor NTC10K

// Parâmetros do NTC10K
#define R_REF 10000.0     // Resistor de 10K em série
#define VCC 3.3           // Tensão de operação do ESP32
#define B_PARAM 3950.0    // Parâmetro Beta do NTC10K
#define T_REF 298.15      // Temperatura de referência (25°C) em Kelvin
#define R_REF_NTC 10000.0 // Resistência de referência do NTC em 25°C

// Inicialização dos Sensores
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  // --- Conexão ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicialização dos Sensores
  sensors.begin();

  // Inicialização do Cartão SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha na inicialização do cartão SD!");
  } else {
    Serial.println("Cartão SD inicializado com sucesso.");
  }
}

void loop() {
  // Leitura dos Sensores
  sensors.requestTemperatures();
  float tempDS18B20 = sensors.getTempCByIndex(0);

  int analogValue = analogRead(NTC_PIN);
  float voltage = analogValue * (VCC / 4095.0); // ESP32 tem ADC de 12 bits
  float resistance = (VCC / voltage - 1) * R_REF;
  float tempKelvin = 1.0 / ((log(resistance / R_REF_NTC) / B_PARAM) + (1.0 / T_REF));
  float tempNTC = tempKelvin - 273.15;

  // Verificação das Leituras
  if (tempDS18B20 == -127.00) {
    Serial.println("Falha ao ler o sensor DS18B20!");
  }
  if (isnan(tempNTC)) {
    Serial.println("Falha ao ler o sensor NTC10K!");
  }

  // Formatação dos Dados
  String dataString = String(millis()) + "," + String(tempDS18B20) + "," + String(tempNTC);

  // Envio dos Dados via Wi-Fi
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(dataString);
    if (httpResponseCode > 0) {
      Serial.printf("Dados enviados. Código de resposta: %d\n", httpResponseCode);
    } else {
      Serial.printf("Erro no envio. Código de resposta: %d\n", httpResponseCode);
    }
    http.end();
  }

  // Gravação no Cartão SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  } else {
    Serial.println("Erro ao abrir datalog.txt");
  }

  // Intervalo de 5 segundos entre as leituras
  delay(5000);
}