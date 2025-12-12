// Arduino - Slave

#include <Wire.h>
#include "DHT.h"
#include <string.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define ARDUINO 1

#define SOUND_SPEED 0.034

const int Trigger = 9;
const int Echo = 5;

char c = 'd';
char* InfoTransmitida = malloc(sizeof(char)*8);
String DadoBruto = String("");

unsigned long DelayUltrassonico;

int EstadoLED = LOW;
int MudarLED = 0;

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
  Serial.println("Dado recebido!");
  while (Wire.available()) {
    c = Wire.read();
    Serial.println(c);
  }
  if (c == 'c' || c == 'C') {
    MudarLED = 1;
  }
}

void RequestEvent() {
  Wire.write(InfoTransmitida);
}

void CalculaDado() {

  if (c == 'a' || c == 'A') {
    // Distância
    if (millis() - DelayUltrassonico > 500) {
      DelayUltrassonico = millis();
      DadoBruto = String(CalculaDistancia());
    }
  } else if (c == 'b' || c == 'B') {
    // Temperatura
    DadoBruto = String(dht.readTemperature());
  } else if (MudarLED == 1 && (c == 'c' || c == 'C')) {
    // LED
    EstadoLED = !EstadoLED;
    digitalWrite(LED_BUILTIN, EstadoLED);
    if (EstadoLED == HIGH) {
      DadoBruto = "ON";
    } else {
      DadoBruto = "OFF";
    }
    MudarLED = 0;
  }
  
  if (c != 'd') {
    if (c != 'c' && c != 'C') {
      while(DadoBruto.length() < 6) {
        DadoBruto.concat(0);
      }
    } else {
      while(DadoBruto.length() < 6) {
        DadoBruto.concat(" ");
      }
    }

    DadoBruto.toCharArray(InfoTransmitida, 7);
    Serial.println(InfoTransmitida);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(ARDUINO);
  dht.begin();
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);
  DelayUltrassonico = millis();
  pinMode(LED_BUILTIN, INPUT);
  digitalWrite(LED_BUILTIN, EstadoLED);
}

void loop() {
  CalculaDado();
}
