// Implemente um robô aspirador com 
// base no diagrama de estados.

const int Idle = 0;
const int Cleaning = 1;
const int Docking = 2;
const int Charging = 3;

const int LED_1 = 12;
const int LED_2 = 13;

int EstadoAtual = Idle;

void setup() {
  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  if (EstadoAtual == Idle){
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    if (Serial.available() > 0 && Serial.read() == 'a') {
      EstadoAtual = Cleaning;
    }
  } else if (EstadoAtual == Cleaning) {
    unsigned long TrocaEstado = millis();
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    while (millis() - TrocaEstado < 2000) {
      if (Serial.available() > 0 && Serial.read() == 'b') {
        break;
      }
    }
    EstadoAtual = Docking;
  } else if (EstadoAtual == Docking) {
    Serial.println("mark docking");
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    delay(2000);
    EstadoAtual = Charging;
  } else if (EstadoAtual == Charging) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    if (Serial.available() > 0 && Serial.read() == 'a') {
      EstadoAtual = Cleaning;
    }
  }
}