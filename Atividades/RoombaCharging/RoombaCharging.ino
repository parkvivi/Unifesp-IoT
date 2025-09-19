// Adicione um terceiro LED ao sistema da atividade anterior (Roomba Subsistemas), 
// conectado ao ESP32. Implemente a seguinte lógica de operação:
// - Em Idle, o LED deve estar sempre ligado.
// - Em Cleaning, o LED deve ir reduzindo a sua luminosidade até zero; A luminosidade 
// deve ser máxima quando entra no estado e deve ser mínima quando sai do estado.
// - Em docking, o LED deve se manter apagado.
// - Em Charging, o LED deve aumentar sua intensidade.

const int EstadoIdle      = 0;
const int EstadoCleaning  = 1;
const int EstadoDocking   = 2;
const int EstadoCharging  = 3;

const int Porta_1 = 15;
const int Porta_2 = 4;

const int LED = 26;
int EstadoLED;

int EstadoAtual, flag;

void setup() {
  Serial.begin(115200);
  pinMode(Porta_1, OUTPUT);
  pinMode(Porta_2, OUTPUT);
  EstadoAtual = EstadoIdle;
}

void loop() {
  if (EstadoAtual == EstadoIdle){
    EstadoLED = 255;
    dacWrite(LED, EstadoLED);
    while (Serial.available() == 0 || (Serial.available() != 0 && Serial.read() != 'a')) {
      Serial.println("IDLE");
      digitalWrite(Porta_1, LOW);
      digitalWrite(Porta_2, LOW); // Idle
    }
    EstadoAtual = EstadoCleaning;
  } else if (EstadoAtual == EstadoCleaning) {
    unsigned long InicioCleaning = millis();
    unsigned long DecresceLED = millis();
    EstadoLED = 255;
    Serial.println("CLEANING");
    while ((Serial.available() == 0 && millis() - InicioCleaning < 2000) || (Serial.available() != 0 && Serial.read() != 'b')) {
      digitalWrite(Porta_1, LOW);
      digitalWrite(Porta_2, HIGH); // Cleaning
      if (2000 / 255 < millis() - DecresceLED){
        DecresceLED = millis();
        dacWrite(LED, EstadoLED);
        --EstadoLED;
      }
    }
    EstadoAtual = EstadoDocking;
  } else if (EstadoAtual == EstadoDocking) {
    Serial.println("DOCKING");
    EstadoLED = 0;
    dacWrite(LED, EstadoLED);
    digitalWrite(Porta_1, HIGH);
    digitalWrite(Porta_2, LOW); // Docking
    delay(2000);
    EstadoAtual = EstadoCharging;
  } else if (EstadoAtual == EstadoCharging) {
    Serial.println("CHARGING");
    unsigned long InicioCharging = millis();
    unsigned long CresceLED = millis();
    EstadoLED = 0;
    flag = 0;
    while (millis() - InicioCharging < 2000 && flag == 0) {
      digitalWrite(Porta_1, HIGH);
      digitalWrite(Porta_2, HIGH); // Charging
      if (2000 / 255 < millis() - CresceLED){
        CresceLED = millis();
        dacWrite(LED, EstadoLED);
        ++EstadoLED;
      }
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