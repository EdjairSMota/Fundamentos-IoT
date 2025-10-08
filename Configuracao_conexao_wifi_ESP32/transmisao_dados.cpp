#include <HTTPClient.h>
#include <WiFi.h> 

// Endereço do Raspberry Pi e o endpoint da API
const char* serverAddress = "http://<IP_DO_RPi>:5000/datalogger";

void loop() {
  // COLOCAR AQUI O CÓDIGO DE LEITURA DOS SENSORES E DO CARTÃO SD
  String dataString = String(millis()) + "," + String(tempDS18B20) + "," + String(tempNTC);

  // Verifica se o ESP32 está conectado ao Wi-Fi antes de tentar enviar
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "text/plain");

    // Envia a requisição POST com a string de dados
    int httpResponseCode = http.POST(dataString);
    if (httpResponseCode > 0) {
      Serial.printf("Dados enviados. Codigo de resposta: %d\n", httpResponseCode);
    } else {
      Serial.printf("Erro no envio. Codigo de resposta: %d\n", httpResponseCode);
    }
    http.end(); // Fecha a conexão
  }
  // RESTANTE DO CÓDIGO
}