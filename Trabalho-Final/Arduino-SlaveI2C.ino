// Arduino - Slave

#include <Wire.h>
#include "DHT.h"
#include <string.h>
#include <Servo.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define ARDUINO 1

#define SOUND_SPEED 0.034

const int Trigger = 9;
const int Echo = 5;

const int LDR = A0;

// SERVO:
#define SERVO_INATIVO 0
#define SERVO_ATIVO 150

Servo servo;
int estadoServo = SERVO_INATIVO;
int mudarPosicaoServo = 0;
const int servoPin = 2;

char c = 'z';
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

void MudaPosServo() {
  estadoServo = (estadoServo == SERVO_ATIVO) ? SERVO_INATIVO : SERVO_ATIVO;
  servo.write(estadoServo);
}

void ReceiveEvent(int bytes) {
  Serial.println("Dado recebido!");
  while (Wire.available()) {
    c = Wire.read();
    Serial.println(c);
    if(c == 'e' || c == 'E') {
      mudarPosicaoServo = 1;
    }
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
    // Umidade
    DadoBruto = String(dht.readHumidity());
  } else if (c == 'c' || c == 'C') {
    // Temperatura
    DadoBruto = String(dht.readTemperature());
  } else if (c == 'd' || c == 'D') {
    // Temperatura
    DadoBruto = String(analogRead(LDR));
  }
  else if (mudarPosicaoServo == 1 && (c == 'e' || c == 'E')) {
    MudaPosServo();
    mudarPosicaoServo = 0;
    DadoBruto = (estadoServo == SERVO_INATIVO) ? String("pos0") : String("pos180");
    Serial.print("DadoBruto: ");
    Serial.println(DadoBruto);
  }
  
  if (c != 'z') {
    while(DadoBruto.length() < 6) {
      DadoBruto.concat(" ");
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
  pinMode(LDR, INPUT);

  // Servos
  servo.attach(servoPin);
  servo.write(estadoServo);
}

void loop() {
  CalculaDado();
}
