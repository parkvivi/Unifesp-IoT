// Crie um programa que leia dois valores em sequência no Monitor
// Serial. Os valores possíveis são ‘0’ ou ‘1’. Em posse desses valores,
// acenda o LED interno da placa seguindo o padrão das portas lógicas
// AND, OR e XOR.

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() == 2) {
    int v1 = Serial.read() - 48;  // ascii '0' = 48
    int v2 = Serial.read() - 48;

    if ((v1 == 0 || v1 == 1) && (v2 == 0 || v2 == 1))
      digitalWrite(LED_BUILTIN, v1 || v2);
  }
}
