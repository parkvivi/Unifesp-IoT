// Atividade 1 - Código Arduino Mega

#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11

DHT SensorUmidade(DHTPIN, DHTTYPE);
const int PortaUmidade = 6;

// LEDs Roomba
const int LEDsRoomba[2] = {12, 13};

// Controle dos LEDs do Roomba
int EstadoLEDsRoomba[2] = {LOW, LOW};
unsigned long PiscaLED[2];

// Barramento ESP32-Arduino: Estados Roomba
const int PortaRoomba[2] = {2, 7};

// Controle de Estados do Roomba
int EstadoAnteriorRoomba[2] = {0, 0};
int EstadoAtualRoomba[2];

// HC-SR04 - Sensor Ultrassônico
const int Trigger = 9;
const int Echo = 5;

// Barramento ESP32-Arduino: Estados HC-SR04 - Sensor Ultrassônico
const int PortaUltrassom[2] = {22, 24};

// Cooldown para Atualização dos Estados
const int TempoCooldown = 500;
unsigned long AtualizaEstados;

#define VELOCIDADE_SOM 0.034

// LDR
const int LDR = A0;
const int PortaLDR = 4;

void Idle() {
  // Apaga LEDs do Roomba
  digitalWrite(LEDsRoomba[0], LOW);
  digitalWrite(LEDsRoomba[1], LOW);
}

void Cleaning() {
  // Apaga LED 2 do Roomba
  EstadoLEDsRoomba[1] = LOW;
  digitalWrite(LEDsRoomba[1], EstadoLEDsRoomba[1]);

  // Troca estado do LED 1 do Roomba
  if (millis() - PiscaLED[0] > 100) {
    PiscaLED[0] = millis();
    EstadoLEDsRoomba[0] = !EstadoLEDsRoomba[0];
    digitalWrite(LEDsRoomba[0], EstadoLEDsRoomba[0]);
  }
}

void Docking() {
  // Apaga LED 1 do Roomba
  EstadoLEDsRoomba[0] = LOW;
  digitalWrite(LEDsRoomba[0], EstadoLEDsRoomba[0]);

  // Troca estado do LED 2 do Roomba
  if (millis() - PiscaLED[1] > 50) {
    PiscaLED[1] = millis();
    EstadoLEDsRoomba[1] = !EstadoLEDsRoomba[1];
    digitalWrite(LEDsRoomba[1], EstadoLEDsRoomba[1]);
  }
}

void Charging() {
  // Troca estado do LED 1 do Roomba
  if (millis() - PiscaLED[0] > 100) {
    PiscaLED[0] = millis();
    EstadoLEDsRoomba[0] = !EstadoLEDsRoomba[0];
    digitalWrite(LEDsRoomba[0], EstadoLEDsRoomba[0]);
  }

  // Troca estado do LED 2 do Roomba
  if (millis() - PiscaLED[1] > 50) {
    PiscaLED[1] = millis();
    EstadoLEDsRoomba[1] = !EstadoLEDsRoomba[1];
    digitalWrite(LEDsRoomba[1], EstadoLEDsRoomba[1]);
  }
}

float DistanciaObstaculo(){

  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
  
  long DuracaoPulso = pulseIn(Echo, HIGH);
  
  float Distancia = (DuracaoPulso * VELOCIDADE_SOM) / 2;

  return Distancia;
}

void setup() {
  Serial.begin(115200);

  // LEDs Roomba
  pinMode(LEDsRoomba[0], OUTPUT);
  pinMode(LEDsRoomba[1], OUTPUT);

  // Pinos Trigger e Echo Sensor Ultrassônico
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  // Barramento ESP32-Arduino: Estados Sensor Ultrassônico
  pinMode(PortaUltrassom[0], OUTPUT);
  pinMode(PortaUltrassom[1], OUTPUT);

  // Barramento ESP32-Arduino: Estados Roomba
  pinMode(PortaRoomba[0], INPUT);
  pinMode(PortaRoomba[1], INPUT);

  PiscaLED[0] = millis();
  PiscaLED[1] = millis();

  // Umidade
  SensorUmidade.begin();
  pinMode(PortaUmidade, OUTPUT);

  // LDR
  pinMode(LDR, INPUT);
  pinMode(PortaLDR, OUTPUT);

  AtualizaEstados = millis();
}

void loop() {
  EstadoAtualRoomba[0] = digitalRead(PortaRoomba[0]);
  EstadoAtualRoomba[1] = digitalRead(PortaRoomba[1]);

  if (EstadoAnteriorRoomba[0] != EstadoAtualRoomba[0] || EstadoAnteriorRoomba[1] != EstadoAtualRoomba[1]) {
    PiscaLED[0] = millis();
    PiscaLED[1] = millis();
    EstadoAnteriorRoomba[0] = EstadoAtualRoomba[0];
    EstadoAnteriorRoomba[1] = EstadoAtualRoomba[1];
  }
  
  if (EstadoAtualRoomba[0] == 0 && EstadoAtualRoomba[1] == 0) {
    Idle();
  } else if (EstadoAtualRoomba[0] == 0 && EstadoAtualRoomba[1] == 1) {
    Cleaning();
  } else if (EstadoAtualRoomba[0] == 1 && EstadoAtualRoomba[1] == 0) {
    Docking();
  } else if (EstadoAtualRoomba[0] == 1 && EstadoAtualRoomba[1] == 1) {
    Charging();
  }

  if (millis() - AtualizaEstados > TempoCooldown){
    // Obstáculo
    float Distancia = DistanciaObstaculo();
    Serial.println(Distancia);

    if(Distancia > 30) {
      digitalWrite(PortaUltrassom[0], LOW);
      digitalWrite(PortaUltrassom[1], LOW);
    } else if (Distancia > 20 && Distancia <= 30) {
      digitalWrite(PortaUltrassom[0], LOW);
      digitalWrite(PortaUltrassom[1], HIGH);
    } else if (Distancia > 10 && Distancia <= 20) {
      digitalWrite(PortaUltrassom[0], HIGH);
      digitalWrite(PortaUltrassom[1], LOW);
    } else if (Distancia > 0 && Distancia <= 10) {
      digitalWrite(PortaUltrassom[0], HIGH);
      digitalWrite(PortaUltrassom[1], HIGH);
    }

    // Umidade
    float Umidade = SensorUmidade.readHumidity();
    Serial.println(Umidade);

    if (Umidade >= 55) {
      digitalWrite(PortaUmidade, HIGH);
    } else {
      digitalWrite(PortaUmidade, LOW);
    }

    // LDR
    int Luz = analogRead(LDR);
    Serial.println(Luz);

    if (Luz <= 850) {
      digitalWrite(PortaLDR, LOW);
    } else {
      digitalWrite(PortaLDR, HIGH);
    }

    Serial.println("===========================");
    AtualizaEstados = millis();
  }

}