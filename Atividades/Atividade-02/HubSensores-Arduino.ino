// Arduino - Slave

#include <Wire.h>
#include "DHT.h"
#include <string.h>

#pragma region I2C

#define ARDUINO 1

char c = 'd';
char InfoTransmitida[9];
String DadoBruto = String("");

#pragma endregion

#pragma region Umidade e Temperatura

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#pragma endregion

#pragma region Ultrassonico

#define SOUND_SPEED 0.034

const int Trigger = 9;
const int Echo = 5;

unsigned long DelayUltrassonico;

#pragma endregion

#pragma region Bluetooth

unsigned long DelayBluetooth;

#pragma endregion

float CalculaDistancia() {
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
  
  long duracao = pulseIn(Echo, HIGH);
  
  float distancia = duracao * SOUND_SPEED/2;

  return distancia;
}

void ReceiveEvent(int bytes) {
  while (Wire.available()) {
    c = Wire.read();
    Serial.println(c);
  }
}

void RequestEvent() {
  Wire.write(InfoTransmitida);
}

void CalculaDado() {
  String Auxiliar = String('%');

  if (c == 'a' || c == 'A') {
    // Temperatura
    DadoBruto = String(dht.readTemperature());
  } else if (c == 'b' || c == 'B') {
    // Umidade
    DadoBruto = String(dht.readHumidity());
  } else if (c == 'c' || c == 'C') {
    // Distância
    if (millis() - DelayUltrassonico > 500) {
      DelayUltrassonico = millis();
      DadoBruto = String(CalculaDistancia());
    }
  }

  if (c != 'd') {
    while(DadoBruto.length() < 6) {
      DadoBruto.concat(0);
    }

    DadoBruto.concat("#");
    Auxiliar.concat(DadoBruto);

    Auxiliar.toCharArray(InfoTransmitida, 9);
    
    Serial.println(InfoTransmitida);
  }
}

void setup() {
  Serial.begin(115200);

  // Módulo Bluetooth
  Serial2.begin(9600);
  DelayBluetooth = millis();

  // I2C
  Wire.begin(ARDUINO);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);

  // Umidade e Temperatura
  dht.begin();

  // Ultrassônico
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  DelayUltrassonico = millis();
}

void loop() {
  CalculaDado();

  if (millis() - DelayBluetooth > 100) {
    Serial2.write(InfoTransmitida);
    DelayBluetooth = millis();
  }
}