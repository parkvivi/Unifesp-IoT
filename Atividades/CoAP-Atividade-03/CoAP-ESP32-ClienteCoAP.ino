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

String distancia = String("");
String temperatura = String("");
String led = String("");

// CLIENTE CoAP
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  int tamPacote = strlen(p);
  String dado = String("");
  String media;

  for (int i = 1; i < tamPacote; i++) {
    dado.concat(p[i]);
  }

  if (p[0] == 'D') {
    float distanciaMedia = dado.toFloat() / 2;
    media = String(distanciaMedia);
    distancia = media;
  } else if (p[0] == 'T') {
    float temperaturaMedia = dado.toFloat() / 2;
    media = String(temperaturaMedia);
    temperatura = media;
  } else if (p[0] == 'L') {
    led = dado;
  }
}

// SERVIDOR HTTP
void handle_OnConnect() {
  distancia = String("###");
  temperatura = String("###");
  led = String("###");
  server.send(200, "text/html", SendHTML(distancia, temperatura, led));
}

void handle_getDistancia() {
  coap.get(IPAddress(192, 168, 0, 3), 5683, "distancia");
  server.send(200, "text/html", SendHTML(distancia, temperatura, led));
}

void handle_getTemperatura() {
  coap.get(IPAddress(192, 168, 0, 3), 5683, "temperatura");
  server.send(200, "text/html", SendHTML(distancia, temperatura, led));
}

void handle_getLED() {
  coap.get(IPAddress(192, 168, 0, 3), 5683, "led");
  server.send(200, "text/html", SendHTML(distancia, temperatura, led));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Não encontrado...");
}

String SendHTML(String distancia, String temperatura, String led){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html{display:flex;text-align:center;align-items:center;justify-content:center;color:#444444;}\n";
  ptr += ".body{margin:50px;display:flex;flex-direction:column;text-align:center;align-items:center;justify-items:center;gap:20px;}\n";
  ptr += ".heading-maior{font-family:serif;font-size:40px;font-weight:bold;margin:0px;}\n";
  ptr += ".heading-media{font-family:serif;font-size:30px;font-weight:bold;margin:0px;}\n";
  ptr += ".heading-menor{font-family:serif;font-size:24px;font-weight:bold;margin:0px;}\n";
  ptr += ".body-text{font-family:sans-serif;font-size:20px;margin:0px;}\n";
  ptr += ".button{color:white;font-size:24px;display:flex;padding:8px 16px;border-radius:8px;border:none;cursor:pointer;}\n";
  ptr += ".button-on{background-color:#0a8b2c;}\n";
  ptr += ".button-on:active{background-color:#0ea536;}\n";
  ptr += ".button-off{background-color:#2c703e;}\n";
  ptr += ".button-off:active{background-color:#3f9d58;}\n";
  ptr += ".div-botoes{width:300px;display:flex;flex-direction: column;gap: 8px;}\n";
  ptr += ".div-botao{background-color:#F5F5F5;display:flex;flex-direction:row;padding:32px;align-items:center;justify-content:space-between;gap:12px;border-radius:8px;}\n";
  ptr += ".div-botao:hover{box-shadow: #444444 0px 0px 8px;transition: box-shadow 0.15s ease-in-out;}\n";
  ptr += ".link{font-family:serif;font-size:16px;color:#1f2a17;text-decoration:underline;}\n";
  ptr += ".link:hover{font-family:serif;font-size:16px;font-weight:bold;color:#1f2a17;text-decoration:underline;transition: font-weight 0.3s;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body class=\"body\">\n";
  ptr += "<h1 class=\"heading-maior\">Webserver de Sensores CoAP</h1>\n";
  ptr += "<h3 class=\"heading-media\">IoT: Atividade 3</h3>\n";
  ptr += "<h6 class=\"heading-menor\">Luiza & Viviane</h3>\n";
  ptr += "<div class=\"div-botoes\">\n";
  ptr += "<div class=\"div-botao\">\n";
  ptr += "<a class=\"button button-on\" href=\"/distancia\">Distancia</a>\n";
  ptr += "<p class=\"body-text\">";
  ptr += distancia;
  ptr += "cm</p>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  ptr += "<a class=\"button button-on\" href=\"/temperatura\">Temperatura</a>\n";
  ptr += "<p class=\"body-text\">";
  ptr += temperatura;
  ptr += "graus </p>\n";
  ptr += "</div>\n";
  ptr += "<div class=\"div-botao\">\n";
  ptr += "<a class=\"button button-on\" href=\"/led\">LED</a>\n";
  ptr += "<p class=\"body-text\">";
  ptr += led;
  ptr += "</p>\n";
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
  server.on("/led", handle_getLED);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP SERVER SUCCESSFULLY STARTED");
}

void loop() {
  server.handleClient();
  coap.loop();
}