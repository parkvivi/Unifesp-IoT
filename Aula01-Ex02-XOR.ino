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
    char v1 = Serial.read();
    char v2 = Serial.read();

    if ((v1 == '0' || v1 == '1') && (v2 == '0' || v2 == '1')) {
      if ((v1 == '0' && v2 == '0') || (v1 == '1' && v2 == '1')) {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
}
