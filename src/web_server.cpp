#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "web_server.h"

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

void setupWebServer() {
  // Define a route for the root URL "/"
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello from ESP32 Web Server!");
  });

  // Start the server
  server.begin();
}
