// Atividade 1 - Código ESP32

const int EstadoIdle      = 0;
const int EstadoCleaning  = 1;
const int EstadoDocking   = 2;
const int EstadoCharging  = 3;

const int PortaRoomba[2] = {15, 4};

const int TempoTransicao = 10000;
const int LEDMaximo = 255;
const int LEDMinimo = 0;

const int LED = 26;
int EstadoLED;

int EstadoAtual, flag;

// HC-SR04 - Sensor Ultrassônico
const int PortaUltrassom[2] = {23, 22};

int EstadoUltrassom[2] = {0, 0};

// Sensor de Umidade
const int PortaUmidade = 25;
int EstadoUmidade;

// LDR
const int PortaLDR = 21;
int EstadoLDR;

// Impressão de Estados a cada 1s
int TempoAtualizacao = 1000;
unsigned long AtualizaEstado;

void ImprimeEstado() {
  // - Estado do sensor ultrassônico (obstáculo)
  EstadoUltrassom[0] = digitalRead(PortaUltrassom[0]);
  EstadoUltrassom[1] = digitalRead(PortaUltrassom[1]);

  if(EstadoUltrassom[0] == 0 && EstadoUltrassom[1] == 0) {
    Serial.println("SEM OBSTÁCULOS");
  }
  else if(EstadoUltrassom[0] == 0 && EstadoUltrassom[1] == 1) {
    Serial.println("OBSTÁCULO LONGE");
  }
  else if(EstadoUltrassom[0] == 1 && EstadoUltrassom[1] == 0) {
    Serial.println("OBSTÁCULO MEIA DISTÂNCIA");
  }
  else if(EstadoUltrassom[0] == 1 && EstadoUltrassom[1] == 1) {
    Serial.println("OBSTÁCULO PERTO");
  }

  // - Estado do sensor de umidade
  EstadoUmidade = digitalRead(PortaUmidade);

  if (EstadoUmidade == 0) {
    Serial.println("UMIDADE ACEITÁVEL");
  } else {
    Serial.println("UMIDADE NÃO ACEITÁVEL");
  }

  // - Estado do LDR
  EstadoLDR = digitalRead(PortaLDR);

  if (EstadoLDR == 0) {
    Serial.println("NADA ACIMA");
  } else {
    Serial.println("OBSTÁCULO ACIMA");
  }

  Serial.println("===========================");
  AtualizaEstado = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(PortaRoomba[0], OUTPUT);
  pinMode(PortaRoomba[1], OUTPUT);

  pinMode(LED, OUTPUT);

  pinMode(PortaUltrassom[0], INPUT);
  pinMode(PortaUltrassom[1], INPUT);

  pinMode(PortaUmidade, INPUT);

  pinMode(PortaLDR, INPUT);

  EstadoAtual = EstadoIdle;
  AtualizaEstado = millis();
}

void loop() {
  if (EstadoAtual == EstadoIdle){
    EstadoLED = 255;
    dacWrite(LED, EstadoLED);
    while (Serial.available() == 0 || (Serial.available() != 0 && Serial.read() != 'a')) {
      digitalWrite(PortaRoomba[0], LOW);
      digitalWrite(PortaRoomba[1], LOW);
      if (millis() - AtualizaEstado > TempoAtualizacao) {
        Serial.println("IDLE");
        ImprimeEstado();
      }
    }
    EstadoAtual = EstadoCleaning;
  } else if (EstadoAtual == EstadoCleaning) {
    unsigned long InicioCleaning = millis();
    unsigned long DecresceLED = millis();
    EstadoLED = LEDMaximo;
    while ((Serial.available() == 0 && millis() - InicioCleaning < TempoTransicao) || (Serial.available() != 0 && Serial.read() != 'b')) {
      digitalWrite(PortaRoomba[0], LOW);
      digitalWrite(PortaRoomba[1], HIGH);
      if (TempoTransicao / LEDMaximo < millis() - DecresceLED){
        DecresceLED = millis();
        dacWrite(LED, EstadoLED);
        --EstadoLED;
      }
      if (millis() - AtualizaEstado > TempoAtualizacao) {
        Serial.println("CLEANING");
        ImprimeEstado();
      }
    }
    EstadoAtual = EstadoDocking;
  } else if (EstadoAtual == EstadoDocking) {
    unsigned long InicioDocking = millis();
    EstadoLED = LEDMinimo;
    dacWrite(LED, EstadoLED);
    while (millis() - InicioDocking < TempoTransicao){
      digitalWrite(PortaRoomba[0], HIGH);
      digitalWrite(PortaRoomba[1], LOW);
      if (millis() - AtualizaEstado > TempoAtualizacao) {
        Serial.println("DOCKING");
        ImprimeEstado();
      }
    }
    EstadoAtual = EstadoCharging;
  } else if (EstadoAtual == EstadoCharging) {
    unsigned long InicioCharging = millis();
    unsigned long CresceLED = millis();
    EstadoLED = LEDMinimo;
    flag = 0;
    while (millis() - InicioCharging < TempoTransicao && flag == 0) {
      digitalWrite(PortaRoomba[0], HIGH);
      digitalWrite(PortaRoomba[1], HIGH);
      if (TempoTransicao / LEDMaximo < millis() - CresceLED){
        CresceLED = millis();
        dacWrite(LED, EstadoLED);
        ++EstadoLED;
      }
      if (millis() - AtualizaEstado > TempoAtualizacao) {
        Serial.println("CHARGING");
        ImprimeEstado();
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