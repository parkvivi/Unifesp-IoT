// Considere o sistema implementado na atividade anterior
// (Roomba). Implemente dois subsistemas: o principal (ESP32) 
// e o secundário (Arduino Mega).

// Código ESP32

const int EstadoIdle      = 0;
const int EstadoCleaning  = 1;
const int EstadoDocking   = 2;
const int EstadoCharging  = 3;

const int Porta_1 = 15;
const int Porta_2 = 4;

int EstadoAtual, flag;

void setup() {
  Serial.begin(115200);
  pinMode(Porta_1, OUTPUT);
  pinMode(Porta_2, OUTPUT);
  EstadoAtual = EstadoIdle;
}

void loop() {
  if (EstadoAtual == EstadoIdle){
    while (Serial.available() == 0 || (Serial.available() != 0 && Serial.read() != 'a')) {
      Serial.println("IDLE");
      digitalWrite(Porta_1, LOW);
      digitalWrite(Porta_2, LOW); // Idle
    }
    EstadoAtual = EstadoCleaning;
  } else if (EstadoAtual == EstadoCleaning) {
    unsigned long InicioCleaning = millis();
    Serial.println("CLEANING");
    while ((Serial.available() == 0 && millis() - InicioCleaning < 2000) || (Serial.available() != 0 && Serial.read() != 'b')) {
      digitalWrite(Porta_1, LOW);
      digitalWrite(Porta_2, HIGH); // Cleaning
    }
    EstadoAtual = EstadoDocking;
  } else if (EstadoAtual == EstadoDocking) {
    Serial.println("DOCKING");
    digitalWrite(Porta_1, HIGH);
    digitalWrite(Porta_2, LOW); // Docking
    delay(2000);
    EstadoAtual = EstadoCharging;
  } else if (EstadoAtual == EstadoCharging) {
    Serial.println("CHARGING");
    unsigned long InicioCharging = millis();
    flag = 0;
    while (millis() - InicioCharging < 2000 && flag == 0) {
      digitalWrite(Porta_1, HIGH);
      digitalWrite(Porta_2, HIGH); // Charging
      if (Serial.available() > 0 && Serial.read() == 'a') {
        flag = 1;
      }
    }
    if (flag == 0) {
      EstadoAtual = EstadoIdle;
    } else {
      EstadoAtual = EstadoCleaning;
    }
  }
}