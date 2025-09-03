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
