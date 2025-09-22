#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Espera a conexão do monitor serial
  }

  Serial.print("Iniciando cartao SD...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha na inicializacao do SD!");
    while (1);
  }
  Serial.println("inicializacao bem-sucedida.");

  // Exemplo de como abrir um arquivo no cartão
  File dataFile = SD.open("teste.txt", FILE_WRITE);

  if (dataFile) {
    Serial.println("Escrevendo em teste.txt...");
    dataFile.println("Ola, mundo!");
    dataFile.close();
    Serial.println("Concluido.");
  } else {
    Serial.println("Erro ao abrir teste.txt");
  }
}

void loop() {
  // Nada a fazer no loop, pois a inicialização e o teste foram feitos no setup()
}