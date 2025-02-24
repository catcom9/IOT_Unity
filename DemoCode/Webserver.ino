/*
#include "Webserver.h"

void WebServer_Start(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  delay(10);
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", Website1 + String(Current_Pos.latitude, 20) + Website2 + String(Current_Pos.longitude, 20) + Website3);
  });

  server.begin();
}
*/