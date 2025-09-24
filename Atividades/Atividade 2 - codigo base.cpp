#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>

// Definições de pinos
#define ONE_WIRE_BUS 2     // Pino para o sensor DS18B20
const int chipSelect = 10; // Pino CS para o leitor de SD

// Definições para o sensor analógico NTC
const int NTC_PIN = A0;
const int B_VALUE = 3950;  // Valor Beta do NTC (verifique o datasheet do seu sensor)
const int R_NTC_NOMINAL = 10000;
const int TEMP_NOMINAL = 25; // 25°C
const int R_SERIES = 10000;  // Resistor em série com o NTC

// Configuração das bibliotecas
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
RTC_DS3231 rtc;

// Variável para controle de tempo de gravação
unsigned long lastLogTime = 0;
const unsigned long logInterval = 600000; // 10 minutos em milissegundos (10 * 60 * 1000)

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Espera a conexão do monitor serial
  }

  // Inicialização do RTC
  if (!rtc.begin()) {
    Serial.println("Erro: Nao foi possivel encontrar o modulo RTC!");
    while (1);
  }

  // Verificação e ajuste da hora do RTC (opcional, mas recomendado)
  if (rtc.lostPower()) {
    Serial.println("RTC perdeu a energia, ajustando a hora com base na hora de compilacao.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Inicialização do Cartão SD
  Serial.print("Iniciando cartao SD...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha na inicializacao do SD! Verifique o hardware.");
    while (1);
  }
  Serial.println("inicializacao bem-sucedida.");

  // Inicialização do DS18B20
  sensors.begin();
  Serial.println("Sensor DS18B20 inicializado.");
  
  // Cria o arquivo de log se ele não existir e adiciona o cabeçalho
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    if (dataFile.size() == 0) { // Verifica se o arquivo está vazio
      dataFile.println("timestamp,temp_ds18b20,temp_ntc");
      Serial.println("Cabecalho do arquivo de log adicionado.");
    }
    dataFile.close();
  } else {
    Serial.println("Erro ao abrir ou criar o arquivo de log.");
  }
}

void loop() {
  // Verifica se o intervalo de 10 minutos já passou
  if (millis() - lastLogTime >= logInterval) {
    lastLogTime = millis();
    
    // --- Leitura e processamento dos dados ---
    
    // 1. Leitura do DS18B20
    sensors.requestTemperatures();
    float tempDS18B20 = sensors.getTempCByIndex(0);
    
    // 2. Leitura do NTC
    int analogValue = analogRead(NTC_PIN);
    float resistance = R_SERIES / ((1023.0 / analogValue) - 1.0);
    float steinhart;
    steinhart = resistance / R_NTC_NOMINAL; // (R/Ro)
    steinhart = log(steinhart);            // ln(R/Ro)
    steinhart /= B_VALUE;                  // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMP_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;           // 1 / [...]
    float tempNTC = steinhart - 273.15;    // - 273.15 para Celsius
    
    // 3. Leitura do timestamp do RTC
    DateTime now = rtc.now();
    String timestamp = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    
    // 4. Formatação e gravação dos dados
    String dataString = timestamp + "," + String(tempDS18B20) + "," + String(tempNTC);
    
    Serial.println(dataString); // Imprime no monitor serial para debug
    
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    } else {
      Serial.println("Erro ao gravar no arquivo. Verifique o cartao SD.");
    }
  }
}