// ESP32 - Servidor CoAP

#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <Wire.h>

const char* ssid     = "sua mãe";
const char* password = "saidaqui";

#define MAX_BYTES 10

const char DISTANCIA = 'a';
const char UMIDADE = 'b';
const char TEMPERATURA = 'c';
const char LED = 'd';
const char SERVO = 'e';

char op = 'z';

// CoAP server endpoint url callback
void callback_distancia(CoapPacket &packet, IPAddress ip, int port);
void callback_umidade(CoapPacket &packet, IPAddress ip, int port);
void callback_temperatura(CoapPacket &packet, IPAddress ip, int port);
void callback_led(CoapPacket &packet, IPAddress ip, int port);

WiFiUDP udp;
Coap coap(udp);

// I2C
void SelecionaInformacao(char c) {
  Wire.beginTransmission(1);
  Wire.write(c);
  delay(100);
  Wire.endTransmission();
  Serial.print("PEDIDO: ");
  if (c == 'a') {
    Serial.println("distância");
  } else if (c == 'b') {
    Serial.println("umidade");
  } else if (c == 'c') {
    Serial.println("temperatura");
  } else if (c == 'd') {
    Serial.println("led");
  } else if (c == 'e') {
    Serial.println("servo");
  }
}

String RecebeInformacao() {
  String dado = String(""); 

  Wire.requestFrom(1, MAX_BYTES);
  delay(100);
  while (Wire.available() > 0) {
    char l = Wire.read();
    dado.concat(l);
  }

  Serial.print("Recebido do Arduino: ");
  Serial.println(dado);
  Serial.print("\n");
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

void callback_umidade(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Received: umidade");

  if (packet.code == COAP_GET) {
    SelecionaInformacao(UMIDADE);
    String dado = String("U");
    String informacaoArduino = RecebeInformacao();
    dado.concat(informacaoArduino);

    Serial.print("Umidade Enviada: ");
    Serial.println(dado);

    char payload[MAX_BYTES + 2];
    dado.toCharArray(payload, MAX_BYTES + 2);
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

    char payload[MAX_BYTES + 2];
    dado.toCharArray(payload, MAX_BYTES + 2);
    coap.sendResponse(ip, port, packet.messageid, payload);
  } else {
    coap.sendResponse(ip, port, packet.messageid, "Not Supported");
  }
}

void callback_led(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Received: led");

  if (packet.code == COAP_GET) {
    String dado = String("L");
    String informacaoArduino = RecebeInformacao();
    dado.concat(informacaoArduino);

    Serial.print("Luminosidade Enviada: ");
    Serial.println(dado);

    char payload[MAX_BYTES + 2];
    dado.toCharArray(payload, MAX_BYTES + 2);
    coap.sendResponse(ip, port, packet.messageid, payload);
  } else if (packet.code == COAP_PUT) {
    SelecionaInformacao(SERVO);
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

  Serial.println("Setup Callback Distância");
  coap.server(callback_distancia, "distancia");

  Serial.println("Setup Callback Umidade");
  coap.server(callback_umidade, "umidade");

  Serial.println("Setup Callback Temperatura");
  coap.server(callback_temperatura, "temperatura");

  Serial.println("Setup Callback LED");
  coap.server(callback_led, "led");

  coap.start();
}

void loop() {
  coap.loop();

  // Apenas para debugging
  if (Serial.available() > 0) {
    op = Serial.read();
    if (op == 'a' || op == 'b' || op == 'c' || op == 'd') {
      SelecionaInformacao(op);
      RecebeInformacao();
    } else if (op == 'e') {
      SelecionaInformacao(op);
    }
  }
}