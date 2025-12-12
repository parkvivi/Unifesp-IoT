// ESP32 - Master

#include <Wire.h>
#include "BluetoothSerial.h"

#pragma region Bluetooth

BluetoothSerial SerialBT;
uint8_t address[6] = {0x98, 0xD3, 0x31, 0xF7, 0x40, 0x3F};

String AuxiliarBluetooth = String("");
float DadoBluetooth = 0;

#pragma endregion

#pragma region I2C

#define MAX_BYTES 8
String AuxiliarI2C = String("");
float DadoI2C = 0;

#pragma endregion

#pragma region Dados

float MediaDados[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int k = 0, flagCalculaMedia = 0;
unsigned long DelayMedia;

#pragma endregion

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
  Wire.requestFrom(1, MAX_BYTES);
  delay(100);

  AuxiliarI2C = String("");
  char c = Wire.read();

  if (c != '%') {
    while (Wire.available() > 0){
      Wire.read();
    }
    Serial.println("Erro");
  } else {
    c = Wire.read();
  }

  while (Wire.available() > 0 && c != '#') {
    AuxiliarI2C.concat(c);
    c = Wire.read();
  }

  while (Wire.available() > 0) {
    Wire.read();
  }

  //Serial.print("Dados I2C: ");
  //Serial.println(AuxiliarI2C);
  DadoI2C = AuxiliarI2C.toFloat();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("HubSensores-Master-LV", true);
  Serial.print("Conectando");
  while(!SerialBT.connect(address)){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Conectou!");
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
      flagCalculaMedia = 1;
      DelayMedia = millis();
    }
  }

  if (SerialBT.available() > 0) {
    AuxiliarBluetooth = String("");
    char e = SerialBT.read();
    
    if (SerialBT.available() > 0 && e == '%') {
      e = SerialBT.read();
    }

    while (SerialBT.available() > 0 && e != '#') {
      AuxiliarBluetooth.concat(e);
      e = SerialBT.read();
    }
    
    if (!AuxiliarBluetooth.equals("")) {
      //Serial.print("Dados Bluetooth: ");
      //Serial.println(AuxiliarBluetooth);
      DadoBluetooth = AuxiliarBluetooth.toFloat();
    }
  }

  MediaDados[k] = (DadoI2C + DadoBluetooth)/2;

  if (k == 9) {
    k = 0;
  } else {
    k++;
  }

  if (flagCalculaMedia == 1 && (millis() - DelayMedia > 500)) {
    flagCalculaMedia = 0;
    float Media = 0;

    // Calcular Media
    for (int j = 0; j < 10; j++) {
      Media += MediaDados[j];
    }

    Media /= 10;

    Serial.print("MÉDIA: ");
    Serial.println(Media);

    ImprimeMenu();
  }

  delay(10);
}