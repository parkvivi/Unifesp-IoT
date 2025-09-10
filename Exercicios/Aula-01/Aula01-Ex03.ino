// Crie um semáforo no Monitor Serial:
// a) O Monitor Serial imprime “VERMELHO”. A cada 1 segundo, um
// ponto deve ser imprimido.
// b) Após 5s, o Monitor Serial deve imprimir “VERDE”. A cada 1
// segundo, um ponto deve ser imprimido.
// c) Após 5s, o Monitor Serial deve imprimir “AMARELO”. A cada 1
// segundo, um ponto deve ser imprimido.
// d) Após 2s, voltar ao passo a).
// Desafio: caso o usuário envie qualquer caractere pelo Monitor Serial,
// deve ser impressa a mensagem “PEDESTRE” e o programa deve voltar ao
// passo a)

int cont = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {

    // Limpa buffer
    while (Serial.available() > 0) {
      Serial.read();
    }

    Serial.println("PEDESTRE");

    // Comportamento diferente dependendo do estado atual
    if (cont >= 0 && cont <= 5) {
      cont = 0;
    } else if (cont > 5 && cont < 10) {
      cont = 10;
    }

  } else {
    switch (cont) {
      case 0:   Serial.println("VERMELHO");
                ++cont;
                break;
      case 5:   Serial.println("VERDE");
                ++cont;
                break;
      case 10:  Serial.println("AMARELO");
                ++cont;
                break;
      case 12:  Serial.println(".");
                cont = 0;
                break;
      default:  Serial.println(".");
                ++cont;
    }
  }

  delay(1000);
}
