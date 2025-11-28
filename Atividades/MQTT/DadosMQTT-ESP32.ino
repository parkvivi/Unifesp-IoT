// ESP32 - Master I2C

#include <Wire.h>
#include <PubSubClient.h>
#include <WiFi.h>

// WiFi
const char *WIFI_SSID = "sua mãe";
const char *PASSWORD = "saidaqui";
const uint16_t MQTT_PORT = 1883;
WiFiClient wifiClient;

// MQTT
const char *MQTT_SERVER = "broker.hivemq.com";
PubSubClient mqttClient(wifiClient);

// Outros
#define MAX_BYTES 6

int EstadoLED = LOW;
int publicar = 0;

char c = 'g';

void ConnectToWiFi(){
  Serial.print("Connecting to WiFi ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConnected to ");
  Serial.println(WIFI_SSID);
}

void ConnectToMqtt(){
 Serial.println("Connecting to MQTT Broker...");
 while (!mqttClient.connected()){
  char clientId[100] = "\0";
  sprintf(clientId, "ESP32Client-LV-%04X", random(0xffff));
  Serial.println(clientId);
  if (mqttClient.connect(clientId)){
    Serial.println("Connected to MQTT broker."); 
		mqttClient.subscribe("/LV-IoT-Unifesp/GRUPO1/resposta"); // recebendo resposta
		mqttClient.subscribe("/LV-IoT-Unifesp/GRUPO2/pedido");  // recebendo pedido
  }
 }
}

// '/LV-IoT-Unifesp/GRUPO1/resposta' recebendo o DADO pedido por NÓS
// '/LV-IoT-Unifesp/GRUPO2/pedido' recebendo o PEDIDO do OUTRO GRUPO

// '/LV-IoT-Unifesp/GRUPO1/pedido' enviando o PEDIDO para o OUTRO GRUPO
// '/LV-IoT-Unifesp/GRUPO2/resposta' enviando o DADO para o OUTRO GRUPO

void CallbackMqtt(char* topic, byte* payload, unsigned int length){
    if (String(topic) == "/LV-IoT-Unifesp/GRUPO1/resposta") {
      Serial.print("RESPOSTA RECEBIDA: "); 
      for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
      }
      Serial.print("\n");
      ImprimeMenu();
    } else if (String(topic) == "/LV-IoT-Unifesp/GRUPO2/pedido") {
        Serial.print("PEDIDO RECEBIDO: "); 
        c = payload[0];
        Serial.println(c);
        publicar = 1;
    }
}

void SetupMqtt(){
 mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
 mqttClient.setCallback(CallbackMqtt);
}

void ImprimeMenu() {
  Serial.print("A - Distância Grupo 1\nB - Temperatura Grupo 1\nC - LED Grupo 1\nD - Distância Grupo 2\nE - Temperatura Grupo 2\nF - LED Grupo 2\n");
}

// Comunicação I2C com Arduino
void SelecionaInformacao(char c) {
  Wire.beginTransmission(1);
  Wire.write(c);
  Wire.endTransmission();
  delay(100);
}

void RecebeInformacao() {
  String v = String(""); 

  Wire.requestFrom(1, MAX_BYTES);
  delay(100);
  while (Wire.available() > 0) {
    v.concat((char) Wire.read());
  }

  char z[7];
  v.toCharArray(z, 7);
  Serial.print("Dado Recebido do Arduino: ");
  Serial.println(z);

  if (publicar == 1) {
    mqttClient.publish("/LV-IoT-Unifesp/GRUPO2/resposta", z);
    publicar = 0;
  }

  Serial.print("\n");
}

void setup() {
  Serial.println("Setup begin");

  Serial.begin(115200);
  Wire.begin();

  ConnectToWiFi();
  SetupMqtt();

  Serial.println("Setup end");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, EstadoLED);

  ImprimeMenu();
}

void loop() {
  if (Serial.available() > 0 || publicar == 1) {
    if (!publicar) {
      c = Serial.read();
      while (Serial.available() > 0) {
        Serial.read();
      }
    }

    if (c == 'a' || c == 'A' || c == 'b' || c == 'B') {
      SelecionaInformacao(c);
      RecebeInformacao();
      ImprimeMenu();
    } else if (c == 'c' || c == 'C') {
      EstadoLED = !EstadoLED;
      digitalWrite(LED_BUILTIN, EstadoLED);
      if (EstadoLED == LOW) {
        Serial.println("LED apagado!");
      } else {
        Serial.println("LED ligado!");
      }
      ImprimeMenu();
    } else if (c == 'd' || c == 'D') {
      mqttClient.publish("/LV-IoT-Unifesp/GRUPO1/pedido", "a");
    } else if (c == 'e' || c == 'E') {
      mqttClient.publish("/LV-IoT-Unifesp/GRUPO1/pedido", "b");
    } else if (c == 'f' || c == 'F') {
      mqttClient.publish("/LV-IoT-Unifesp/GRUPO1/pedido", "c");
    }

    c = 'g';
  }

  if (!mqttClient.connected()){
      ConnectToMqtt();
   }
  mqttClient.loop();
  
	delay(500);

}
