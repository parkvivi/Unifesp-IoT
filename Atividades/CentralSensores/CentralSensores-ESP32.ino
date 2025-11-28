// ESP32 - Master

#include <Wire.h>

#define MAX_BYTES 6

void ImprimeMenu() {
  Serial.print("A - Temperatura\nB - Umidade\nC - Distância\n");
}

void SelecionaInformacao(char c) {
  Wire.beginTransmission(1);
  Wire.write(c);
  Wire.endTransmission();
  delay(100);
}

void RecebeInformacao() {
  char c; 

  Wire.requestFrom(1, MAX_BYTES);
  delay(100);
  while (Wire.available() > 0) {
    c = Wire.read();
    Serial.print(c);
  }

  Serial.print("\n");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  ImprimeMenu();
}

void loop() {
  char c = 'd';

  if (Serial.available() > 0) {
    c = Serial.read();

    while (Serial.available() > 0) {
      Serial.read();
    }

    if (c == 'a' || c == 'A' || c == 'b' || c == 'B' || c == 'c' || c == 'C') {
      SelecionaInformacao(c);
      RecebeInformacao();
      ImprimeMenu();
    }

  }

}
