// ESP32 - Servidor CoAP

#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <Wire.h>

const char* ssid     = "sua mãe";
const char* password = "saidaqui";

#define MAX_BYTES 6

const char DISTANCIA = 'a';
const char TEMPERATURA = 'b';
const char LED = 'c';

int EstadoLED = LOW;

// TEMPORÁRIO
char op = 'd';

// CoAP server endpoint url callback
void callback_distancia(CoapPacket &packet, IPAddress ip, int port);
void callback_temperatura(CoapPacket &packet, IPAddress ip, int port);
void callback_led(CoapPacket &packet, IPAddress ip, int port);

WiFiUDP udp;
Coap coap(udp);

// I2C
void SelecionaInformacao(char c) {
  Wire.beginTransmission(1);
  Wire.write(c);
  Serial.println("Mandando pedido para o Arduino...");
  Wire.endTransmission();
  delay(100);
}

String RecebeInformacao() {
  String dado = String(""); 

  Serial.println("Recebendo dado do Arduino...");

  Wire.requestFrom(1, MAX_BYTES);
  delay(100);
  while (Wire.available() > 0) {
    char l = Wire.read();
    dado.concat(l);
  }

  Serial.print("Dado recebido do Arduino");
  Serial.println(dado);
  return dado;
}

// Endpoints - Servidor CoAP
void callback_distancia(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Received: distancia");

  if (packet.code == COAP_GET) {
    SelecionaInformacao(DISTANCIA);
    String dado = String("D");
    String informacaoArduino = RecebeInformacao();
    dado.concat(informacaoArduino);

    Serial.print("Distância Enviada: ");
    Serial.println(dado);

    char payload[MAX_BYTES + 1];
    dado.toCharArray(payload, MAX_BYTES + 1);
    coap.sendResponse(ip, port, packet.messageid, payload);
  } else {
    coap.sendResponse(ip, port, packet.messageid, "Not Supported");
  }
}

void callback_temperatura(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Received: temperatura");

  if (packet.code == COAP_GET) {
    SelecionaInformacao(TEMPERATURA);
    String dado = String("T");
    String informacaoArduino = RecebeInformacao();
    dado.concat(informacaoArduino);

    Serial.print("Temperatura Enviada: ");
    Serial.println(dado);

    char payload[MAX_BYTES + 1];
    dado.toCharArray(payload, MAX_BYTES + 1);
    coap.sendResponse(ip, port, packet.messageid, payload);
  } else {
    coap.sendResponse(ip, port, packet.messageid, "Not Supported");
  }
}

void callback_led(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Received: led");

  if (packet.code == COAP_GET) {
    EstadoLED = !EstadoLED;
    Serial.println(EstadoLED);
    digitalWrite(LED_BUILTIN, EstadoLED);
    String dado;

    if (EstadoLED == LOW) {
      dado = String("LOFF");
    } else {
      dado = String("LON");
    }

    Serial.print("Status LED Enviado: ");
    Serial.println(dado);

    char payload[MAX_BYTES + 1];
    dado.toCharArray(payload, MAX_BYTES + 1);
    coap.sendResponse(ip, port, packet.messageid, payload);
  } else {
    coap.sendResponse(ip, port, packet.messageid, "Not Supported");
  }
}

// Setup e Loop
void setup() {
  Serial.begin(9600);
  Wire.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, EstadoLED);

  Serial.println("Setup Callback Distância");
  coap.server(callback_distancia, "distancia");

  Serial.println("Setup Callback Temperatura");
  coap.server(callback_temperatura, "temperatura");

  Serial.println("Setup Callback LED");
  coap.server(callback_led, "led");

  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();

  // Apenas para debugging
  if (Serial.available() > 0) {
    op = Serial.read();
    if (op == 'a' || op == 'b' || op == 'c') {
      SelecionaInformacao(op);
      RecebeInformacao();
    }
  }
}