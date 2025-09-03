// Código para o Arduino ler a tensão do divisor e a converta em temperatura. 
// O cálculo é baseado na equação de Steinhart-Hart, uma das fórmulas mais precisas para termistores.

// Definição dos pinos e constantes
const int pinoNTC = A0; // Pino analógico onde o sensor está conectado
const int resistenciaFixa = 10000; // Valor do resistor fixo (em ohms)
const int valorBeta = 3950; // Constante Beta do NTC (consultar o datasheet do sensor que o grupo recebeu)
const int temperaturaPadrao = 25; // Temperatura de referência (25°C)

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial para exibir os dados
}

void loop() {
  // Leitura do valor analógico
  int leituraAnalogica = analogRead(pinoNTC);

  // Conversão da leitura para tensão
  float tensaoAnalogica = (leituraAnalogica * 5.0) / 1024.0;

  // Cálculo da resistência do NTC
  float resistenciaNTC = (5.0 / tensaoAnalogica - 1) * resistenciaFixa;

  // Cálculo da temperatura usando a equação de Steinhart-Hart
  float logResistencia = log(resistenciaNTC);
  float temperaturaKelvin = 1.0 / (1.0/ (temperaturaPadrao + 273.15) + (1.0 / valorBeta) * logResistencia);

  // Conversão de Kelvin para Celsius
  float temperaturaCelsius = temperaturaKelvin - 273.15;

  // Exibição dos resultados no monitor serial
  Serial.print("Leitura analógica: ");
  Serial.println(leituraAnalogica);
  Serial.print("Resistência NTC: ");
  Serial.print(resistenciaNTC);
  Serial.println(" Ohms");
  Serial.print("Temperatura: ");
  Serial.print(temperaturaCelsius);
  Serial.println(" °C");
  Serial.println("--------------------");

  delay(2000); // Aguarda 2 segundos antes de fazer a próxima leitura
}