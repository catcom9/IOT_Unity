#include "Webserver.h"

void WebServer_Start(){
  WiFi.begin(ssid1, password1);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", Website1 + String(Current_Pos.latitude, 10) + Website2 + String(Current_Pos.longitude, 10) + Website3);
  });

  server.begin();
}
