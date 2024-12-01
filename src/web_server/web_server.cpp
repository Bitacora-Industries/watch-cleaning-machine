#include "web_server.h"
#include "config.h"
#include "logger.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

// Define the built-in LED pin (usually GPIO2 on ESP32)
const int LED_PIN = 2;

// Tracks the current state of the LED
bool isLedOn = false;

void initializeRoutes() {
  // Toggle route
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Toggle the LED state
    isLedOn = !isLedOn;
    digitalWrite(LED_PIN, isLedOn ? HIGH : LOW);

    logInfo("LED toggled %s.", isLedOn ? "ON" : "OFF");

    // Send response
    String response = String("LED is now ") + (isLedOn ? "ON" : "OFF");
    request->send(200, "text/plain", response);
  });

  logInfo("Routes initialized.");
}


void setupWebServer() {
  // Initialize the LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure the LED starts in the OFF state

  initializeRoutes();
  server.begin();
  logInfo("Web server initialized on port 80.");
}
