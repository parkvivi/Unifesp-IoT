// Implemente um semáforo. Faça o diagrama de estados.
// Os tempos de estadia em cada led devem ser dados por
// - Vermelho: 2 s
// - Amarelo: 1s
// - Verde: 2 s

const int LEDVermelho = 11;
const int LEDAmarelo = 12;
const int LEDVerde = 13;

const int LUZ_VERMELHA = 0;
const int LUZ_AMARELA = 1;
const int LUZ_VERDE = 2;

int EstadoAtual = LUZ_VERMELHA;

void setup() {
  pinMode(LEDVermelho, OUTPUT);
  pinMode(LEDAmarelo, OUTPUT);
  pinMode(LEDVerde, OUTPUT);
}

void loop() {
  switch (EstadoAtual) {
    case LUZ_VERMELHA:
      digitalWrite(LEDVermelho, HIGH);
      digitalWrite(LEDAmarelo, LOW);
      digitalWrite(LEDVerde, LOW);
      EstadoAtual = LUZ_VERDE;
      delay(2000);
      break;
    case LUZ_AMARELA:
      digitalWrite(LEDVermelho, LOW);
      digitalWrite(LEDAmarelo, HIGH);
      digitalWrite(LEDVerde, LOW);
      EstadoAtual = LUZ_VERMELHA;
      delay(1000);
      break;
    case LUZ_VERDE:
      digitalWrite(LEDVermelho, LOW);
      digitalWrite(LEDAmarelo, LOW);
      digitalWrite(LEDVerde, HIGH);
      EstadoAtual = LUZ_AMARELA;
      delay(2000);
      break;
  }
}
