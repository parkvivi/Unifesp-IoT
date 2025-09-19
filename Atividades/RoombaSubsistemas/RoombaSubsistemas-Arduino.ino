// Considere o sistema implementado na atividade anterior
// (Roomba). Implemente dois subsistemas: o principal (ESP32) 
// e o secundário (Arduino Mega).

// Código Arduino Mega

const int LED_1 = 12;
const int LED_2 = 13; 
const int Porta_1 = 2;
const int Porta_2 = 7;

int EstadoLED_1 = LOW;
int EstadoLED_2 = LOW;

int EstadoAnterior_1 = 0;
int EstadoAnterior_2 = 0;

int Entrada_1;
int Entrada_2;

unsigned long PiscaLED_1;
unsigned long PiscaLED_2;

void Idle() {
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
}

void Cleaning() {
  EstadoLED_2 = LOW;
  digitalWrite(LED_2, EstadoLED_2);
  if (millis() - PiscaLED_1 > 100) {
    PiscaLED_1 = millis();
    EstadoLED_1 = !EstadoLED_1;
    digitalWrite(LED_1, EstadoLED_1);
  }
}

void Docking() {
  EstadoLED_1 = LOW;
  digitalWrite(LED_1, EstadoLED_1);
  if (millis() - PiscaLED_2 > 50) {
    PiscaLED_2 = millis();
    EstadoLED_2 = !EstadoLED_2;
    digitalWrite(LED_2, EstadoLED_2);
  }
}

void Charging() {
  if (millis() - PiscaLED_1 > 100) {
    PiscaLED_1 = millis();
    EstadoLED_1 = !EstadoLED_1;
    digitalWrite(LED_1, EstadoLED_1);
  }
  if (millis() - PiscaLED_2 > 50) {
    PiscaLED_2 = millis();
    EstadoLED_2 = !EstadoLED_2;
    digitalWrite(LED_2, EstadoLED_2);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(Porta_1, INPUT);
  pinMode(Porta_2, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  PiscaLED_1 = millis();
  PiscaLED_2 = millis();
}

void loop() {
  Entrada_1 = digitalRead(Porta_1);
  Entrada_2 = digitalRead(Porta_2);

  if (EstadoAnterior_1 != Entrada_1 || EstadoAnterior_2 !=Entrada_2) {
    PiscaLED_1 = millis();
    PiscaLED_2 = millis();
    EstadoAnterior_1 = Entrada_1;
    EstadoAnterior_2 = Entrada_2;
  }
  
  if(Entrada_1 == 0 && Entrada_2 == 0) {
    Serial.println("IDLE");
    Idle();
  } else if(Entrada_1 == 0 && Entrada_2 == 1) {
    Serial.println("CLEANING");
    Cleaning();
  } else if(Entrada_1 == 1 && Entrada_2 == 0) {
    Serial.println("DOCKING");
    Docking();
  } else {
    Serial.println("CHARGING");
    Charging();
  }
}