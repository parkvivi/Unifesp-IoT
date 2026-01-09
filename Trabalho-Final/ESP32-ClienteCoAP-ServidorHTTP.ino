// ESP32 - Cliente CoAP

#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <WebServer.h>

const char* ssid     = "sua mãe";
const char* password = "saidaqui";

WiFiUDP udp;
Coap coap(udp);

WebServer server(80);

#define BUTTON_PIN 21
int ultimoEstadoBotao = HIGH;
int estadoAtualBotao;

unsigned long DelayRequisicao;

#define PortaLEDVermelho 33
#define PortaLEDVerde 26

int EstadoLEDVermelho = LOW;
int EstadoLEDVerde = HIGH;

String distancia = String("###");
String temperatura = String("###");
String luz = String("###");
String umidade = String("###");

// CLIENTE CoAP
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  if(packet.payloadlen == 0) return; // Se servidor CoAP responder vazio

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  int tamPacote = strlen(p);
  String dado = String("");

  for (int i = 1; i < tamPacote; i++) {
    dado.concat(p[i]);
  }

  if (p[0] == 'D') {
    distancia = dado; // Perto, Normal ou Longe
  } else if (p[0] == 'T') {
    temperatura = dado; // Frio, Normal ou Quente
  } else if (p[0] == 'L') {
    luz = dado; // Claro, Normal ou Escuro
  } else if (p[0] == 'U') {
    umidade = dado; // Seco, Normal ou Úmido
  }
}

// SERVIDOR HTTP
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_getDistancia() {
  coap.get(IPAddress(172, 20, 10, 3), 5683, "distancia");
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_getTemperatura() {
  coap.get(IPAddress(172, 20, 10, 3), 5683, "temperatura");
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_getLuz() {
  coap.get(IPAddress(172, 20, 10, 3), 5683, "led");
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_getUmidade() {
  coap.get(IPAddress(172, 20, 10, 3), 5683, "umidade");
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_changeServo() {
  coap.put(IPAddress(172, 20, 10, 3), 5683, "led", "1");
  server.send(200, "text/html", SendHTML(distancia, temperatura, luz, umidade));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Não encontrado...");
}

void CalculaEstadoLEDs() {
  if(luz == "Claro" || luz == "Escuro") {
    EstadoLEDVermelho = HIGH;
    EstadoLEDVerde = LOW;
    digitalWrite(PortaLEDVermelho, EstadoLEDVermelho);
    digitalWrite(PortaLEDVerde, EstadoLEDVerde);
  } else {
    EstadoLEDVermelho = LOW;
    EstadoLEDVerde = HIGH;
    digitalWrite(PortaLEDVermelho, EstadoLEDVermelho);
    digitalWrite(PortaLEDVerde, EstadoLEDVerde);
  }
}

String SendHTML(String distancia, String temperatura, String luz, String umidade){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>SOS: Amazonia</title>\n";
  ptr += "<style>html{display:flex;text-align:center;align-items:center;justify-content:center;color:#444444;}\n";
  ptr += ".body{margin:50px;display:flex;flex-direction:column;text-align:center;justify-items:center;gap:20px;}\n";
  ptr += ".heading-maior{font-family:serif;font-size:40px;font-weight:bold;margin:0px;}\n";
  ptr += ".heading-media{font-family:serif;font-size:30px;font-weight:bold;margin:0px;}\n";
  ptr += ".heading-menor{font-family:serif;font-size:24px;font-weight:bold;margin:0px;}\n";
  ptr += ".body-text{font-family:sans-serif;font-size:20px;margin:0px;}\n";
  ptr += ".button{color:white;font-size:24px;display:flex;padding:8px 16px;border-radius:8px;border:none;}\n";
  ptr += ".button-ok{background-color:#0a8b2c;}\n";
  ptr += ".button-medo{background-color: #a1a10b;}\n";
  ptr += ".button-perigo{background-color: #8b0a0e;}\n";
  ptr += ".div-botoes{display:flex;flex-direction: row;gap: 8px;}\n";
  ptr += ".div-botao{background-color:#F5F5F5;display:flex;flex-direction:column;padding:30px;align-items:center;justify-content:space-between;gap:15px;border-radius:8px;}\n";
  ptr += ".div-botao:hover{box-shadow: #444444 0px 0px 8px;transition: box-shadow 0.15s ease-in-out;}\n";
  ptr += ".link{font-family:serif;font-size:16px;color:#1f2a17;text-decoration:underline;}\n";
  ptr += ".link:hover{font-family:serif;font-size:16px;font-weight:bold;color:#1f2a17;text-decoration:underline;transition: font-weight 0.3s;}\n";
  ptr += ".div-cabecalho{display: flex;gap: 20px; align-items: center; justify-content: center;}\n";
  ptr += ".imagem-onca{border-radius:8px; width: 150px; height: 150px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body class=\"body\">\n";
  ptr += "<div class=\"div-cabecalho\">\n";
  ptr += "<div>\n";
  ptr += "<h1 class=\"heading-maior\">Habitat da Onca - ProAnimais</h1>\n";
  ptr += "<h3 class=\"heading-media\">IoT: Trabalho Final</h3>\n";
  ptr += "<h6 class=\"heading-menor\">Luiza & Viviane</h3>\n";
  ptr += "</div>\n";
  ptr += "<img class=\"imagem-onca\" src=\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSfVXUdSnCQYx7RHAULyRenVJzjmWYO3RLBAw&s\">\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botoes\">\n";
  ptr += "<div class=\"div-botao\">\n";
  if (distancia == "Perto")
    ptr += "<div class=\"button button-perigo\">Inimigo</div>\n";
  else if (distancia == "Normal")
    ptr += "<div class=\"button button-medo\">Inimigo</div>\n";
  else if (distancia == "Longe")
    ptr += "<div class=\"button button-ok\">Inimigo</div>\n";
  ptr += "<p class=\"body-text\">";
  ptr += distancia;
  ptr += "</p>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  if (temperatura == "Frio")
    ptr += "<div class=\"button button-perigo\">Temperatura</div>\n";
  else if (temperatura == "Normal")
    ptr += "<div class=\"button button-ok\">Temperatura</div>\n";
  else if (temperatura == "Quente")
    ptr += "<div class=\"button button-perigo\">Temperatura</div>\n";
  ptr += "<p class=\"body-text\">";
  ptr += temperatura;
  ptr += "</p>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  if (luz == "Claro")
    ptr += "<div class=\"button button-perigo\">Luz</div>\n";
  else if (luz == "Normal")
    ptr += "<div class=\"button button-ok\">Luz</div>\n";
  else if (luz == "Escuro")
    ptr += "<div class=\"button button-perigo\">Luz</div>\n";
  ptr += "<p class=\"body-text\">";
  ptr += luz;
  ptr += "</p>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  if (umidade == "Seco")
    ptr += "<div class=\"button button-perigo\">Umidade</div>\n";
  else if (umidade == "Normal")
    ptr += "<div class=\"button button-ok\">Umidade</div>\n";
  else if (umidade == "Umido")
    ptr += "<div class=\"button button-perigo\">Umidade</div>\n";
  ptr += "<p class=\"body-text\">";
  ptr += umidade;
  ptr += "</p>\n";
  ptr += "</div>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  if(EstadoLEDVerde == HIGH){
    ptr += "<div class=\"button button-ok\">LED Verde</div>\n";
    ptr += "<p class=\"body-text\">";
    ptr += "Aceso";
    ptr += "</p>\n";
    ptr += "</div>\n";
  } 
  else if(EstadoLEDVerde == LOW) {
    ptr += "<div class=\"button button-ok\">LED Verde</div>\n";
    ptr += "<p class=\"body-text\">";
    ptr += "Apagado";
    ptr += "</p>\n";
    ptr += "</div>\n";
  }
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  if(EstadoLEDVermelho == HIGH){
    ptr += "<div class=\"button button-perigo\">LED Vermelho</div>\n";
    ptr += "<p class=\"body-text\">";
    ptr += "Aceso";
    ptr += "</p>\n";
    ptr += "</div>\n";
  } 
  else if(EstadoLEDVermelho == LOW) {
    ptr += "<div class=\"button button-perigo\">LED Vermelho</div>\n";
    ptr += "<p class=\"body-text\">";
    ptr += "Apagado";
    ptr += "</p>\n";
    ptr += "</div>\n";
  }
  ptr += "</div>\n";
  ptr += "</div>\n";
  ptr += "<p>Patrocinado pelo <a class=\"link\" href=\"https://livramente.vercel.app/\" target=\"_blank\">LivraMente</a></p>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

// SETUP E LOOP
void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(PortaLEDVermelho, OUTPUT);
  pinMode(PortaLEDVerde, OUTPUT);

  digitalWrite(PortaLEDVermelho, EstadoLEDVermelho);
  digitalWrite(PortaLEDVerde, EstadoLEDVerde);

  // WiFi
  Serial.println("-> Handling WiFi Connection...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // CoAP
  Serial.println("-> Handling CoAP Client");
  Serial.println("Setup Response Callback");
  coap.response(callback_response);
  coap.start();
  Serial.println("CoAP SUCCESSFULLY SET UP");


  // Servidor HTTP
  Serial.println("Handling HTTP Server");
  server.on("/", handle_OnConnect);
  server.on("/distancia", handle_getDistancia);
  server.on("/temperatura", handle_getTemperatura);
  server.on("/led", handle_getLuz);
  server.on("/umidade", handle_getUmidade);
  server.on("/servo", handle_changeServo); // Rota para o servo
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP SERVER SUCCESSFULLY STARTED");

  DelayRequisicao = millis();
}

void loop() {
  server.handleClient();
  coap.loop();

  estadoAtualBotao = digitalRead(BUTTON_PIN);

  if(ultimoEstadoBotao == LOW && estadoAtualBotao == HIGH){
    handle_changeServo();
  }

  ultimoEstadoBotao = estadoAtualBotao;

  if(millis() - DelayRequisicao > 3000) {
    DelayRequisicao = millis();
    handle_getDistancia();
    handle_getTemperatura();
    handle_getLuz();
    handle_getUmidade();
  }

  CalculaEstadoLEDs();
}