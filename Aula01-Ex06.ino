// No semáforo implementado no exercício 5, faça a seguinte alteração:
// Caso algum dígito seja recebido pelo Monitor Serial e o
// Estado for Vermelho, então fazer a transição para o Estado Verde

// Portas dos LEDs
const int LEDVermelho = 11;
const int LEDAmarelo = 12;
const int LEDVerde = 13;

// Estados
const int LUZ_VERMELHA = 0;
const int LUZ_AMARELA = 1;
const int LUZ_VERDE = 2;

void setup() {
  Serial.begin(115200);
  pinMode(LEDVermelho, OUTPUT);
  pinMode(LEDAmarelo, OUTPUT);
  pinMode(LEDVerde, OUTPUT);
}

void loop() {
  int EstadoAtual = 0;
  while (EstadoAtual < 3) {
    switch (EstadoAtual) {
      case LUZ_VERMELHA:
        digitalWrite(LEDVermelho, HIGH);
        digitalWrite(LEDAmarelo, LOW);
        digitalWrite(LEDVerde, LOW);
        delay(2000);
        break;
      case LUZ_AMARELA:
        digitalWrite(LEDVermelho, LOW);
        digitalWrite(LEDAmarelo, HIGH);
        digitalWrite(LEDVerde, LOW);
        delay(1000);
        break;
      case LUZ_VERDE:
        digitalWrite(LEDVermelho, LOW);
        digitalWrite(LEDAmarelo, LOW);
        digitalWrite(LEDVerde, HIGH);
        delay(2000);
        break;
    }
    if (Serial.available() > 0 && EstadoAtual == LUZ_VERMELHA) {
      while (Serial.available() > 0) {
        Serial.read();
      }
      EstadoAtual = LUZ_VERDE;
    } else {
      while (Serial.available() > 0) {
        Serial.read();
      }
      ++EstadoAtual;
    }
  }
}