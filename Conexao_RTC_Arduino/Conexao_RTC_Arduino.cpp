#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Espera a conexão do monitor serial
  }

  // Inicializa a comunicação I2C
  Wire.begin();

  // Verifica se o módulo RTC está conectado
  if (!rtc.begin()) {
    Serial.println("Erro: Nao foi possivel encontrar o modulo RTC!");
    Serial.flush();
    while (1) rtc.begin(); // Loop infinito em caso de erro
  }

  // Verifica se o RTC perdeu a energia (para ajuste inicial da hora)
  if (rtc.lostPower()) {
    Serial.println("RTC perdeu a energia, ajustando a hora para a de compilacao!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // Obtém a data e hora atuais do RTC
  DateTime now = rtc.now();

  // Imprime a data e hora no monitor serial
  Serial.print("Data e Hora: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(3000); // Espera 3 segundos para a proxima leitura
}