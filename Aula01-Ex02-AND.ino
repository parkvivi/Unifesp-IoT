// OR

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() == 2) {
    char v1 = Serial.read();
    char v2 = Serial.read();

    if ((v1 == '0' || v1 == '1') && (v2 == '0' || v2 == '1')) {
      if (v1 == '0' && v2 == '0') {
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }
}
