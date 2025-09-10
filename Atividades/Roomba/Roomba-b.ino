// Implemente as seguintes modificações:
// - Crie as seguintes funções para cada estado
// - void idle()
// - void cleaning()
// - void docking()
// - void charging()
// OBS: as regras de transição de estados
// devem fica “fora” das funções

// Especificações de cada função:
// - Em idle(): somente desligar os dois leds
// - Em cleaning(): o LED 1 deve acender/apagar a cada 100 ms e o
// LED 2 deve se manter apagado
// - Em docking(): o LED 2 deve acender/apagar a cada 50 ms e o
// LED 1 deve se manter apagado
// - Em charging(): o LED 1 deve acender/apagar a cada 100 ms e o
// LED 2 deve acender/apagar a cada 50 ms

const int EstadoIdle      = 0;
const int EstadoCleaning  = 1;
const int EstadoDocking   = 2;
const int EstadoCharging  = 3;

const int LED_1 = 12; // Amarelo
const int LED_2 = 13; // Verde

int EstadoAtual = EstadoIdle;

void Idle() {
  Serial.println("IDLE");
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  while (Serial.available() == 0 || (Serial.available() > 0 && Serial.read() != 'a')) {}
}

void Cleaning() {
  Serial.println("CLEANING");
  unsigned long InicioCleaning = millis();
  unsigned long PiscaLED = millis();
  int EstadoLED_1 = LOW;
  digitalWrite(LED_1, EstadoLED_1);
  digitalWrite(LED_2, LOW);
  while (millis() - InicioCleaning < 2000) {
    if (millis() - PiscaLED > 100) {
      PiscaLED = millis();
      EstadoLED_1 = !EstadoLED_1;
      digitalWrite(LED_1, EstadoLED_1);
    }
    if (Serial.available() > 0 && Serial.read() == 'b') {
        break;
    }
  }
}

void Docking() {
  Serial.println("DOCKING");
  unsigned long InicioDocking = millis();
  unsigned long PiscaLED = millis();
  int EstadoLED_2 = LOW;
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, EstadoLED_2);
  while (millis() - InicioDocking < 2000) {
    if (millis() - PiscaLED > 50) {
      PiscaLED = millis();
      EstadoLED_2 = !EstadoLED_2;
      digitalWrite(LED_2, EstadoLED_2);
    }
  }
}

int Charging() {
  Serial.println("CHARGING");
  unsigned long InicioCharging = millis();
  unsigned long PiscaLED_1 = millis(); 
  unsigned long PiscaLED_2 = millis();
  int EstadoLED_1 = LOW;
  int EstadoLED_2 = LOW;
  digitalWrite(LED_1, EstadoLED_1);
  digitalWrite(LED_2, EstadoLED_2);
  while (millis() - InicioCharging < 2000) {
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
    if (Serial.available() > 0 && Serial.read() == 'a') {
      return EstadoCleaning;
    }
  }
  return EstadoIdle;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  if (EstadoAtual == EstadoIdle){
    Idle();
    EstadoAtual = EstadoCleaning;
  } else if (EstadoAtual == EstadoCleaning) {
    Cleaning();
    EstadoAtual = EstadoDocking;
  } else if (EstadoAtual == EstadoDocking) {
    Docking();
    EstadoAtual = EstadoCharging;
  } else if (EstadoAtual == EstadoCharging) {
    EstadoAtual = Charging();
  }
}