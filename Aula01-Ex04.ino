// Implementar uma aplicação descrita por um 
// diagrama de estados usando dois LEDs. Mudar
// transições para 3000, 2000 e 1000 ms.

const int Idle = 0;
const int Scanning = 1;
const int Executing = 2;

const int LED_1 = 12;
const int LED_2 = 13;

const int t = 2000;

int EstadoAtual = Idle;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  switch(EstadoAtual) {
    case Idle:
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      EstadoAtual = Scanning;
      break;
    case Scanning:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      EstadoAtual = Executing;
      break;
    case Executing:
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, HIGH);
      EstadoAtual = Idle;
      break;
  }
  delay(t);
}
