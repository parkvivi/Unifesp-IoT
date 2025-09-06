// Implemente um semáforo. Faça o diagrama de estados.
// Os tempos de estadia em cada led devem ser dados por
// - Vermelho: 2 s
// - Amarelo: 1s
// - Verde: 2 s

// Portas dos LEDs
const int LEDVermelho = 11;
const int LEDAmarelo = 12;
const int LEDVerde = 13;

// Estados
const int LUZ_VERMELHA = 0;
const int LUZ_AMARELA = 1;
const int LUZ_VERDE = 2;

void setup() {
  pinMode(LEDVermelho, OUTPUT);
  pinMode(LEDAmarelo, OUTPUT);
  pinMode(LEDVerde, OUTPUT);
}

void loop() {
  for (int EstadoAtual = 0; EstadoAtual < 3; ++EstadoAtual) {
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
  }
}