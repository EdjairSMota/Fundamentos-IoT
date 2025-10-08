#include <WiFi.h>

// Configuração da rede Wi-Fi
const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";

void setup() {
  Serial.begin(115200);

  // Inicia a conexão Wi-Fi
  WiFi.begin(ssid, password); // Inicia o processo de conexão
  Serial.print("Conectando ao WiFi...");

  // Aguarda a conexão ser estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Verifica o status da conexão a cada segundo e só avança quando a conexão estiver estabelecida
    Serial.print(".");
  }

  // Se a conexão for bem sucedida
  Serial.println("\nConectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP()); // Exibe o endereço IP que o ESP32 recebeu na rede, o que é útil para depuração
}