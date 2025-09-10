// Crie um programa que leia um valor pelo Monitor Serial. Se o valor for
// igual a ‘a’, o LED interno da placa deve acender. Se o valor for igual a
// ‘b’, o LED deve apagar.

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if(Serial.available() > 0) {
    char input = Serial.read();
    if(input == 'a') {
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (input == 'b') {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
