#include "config.h"
#include "logger.h"
#include "mdns_manager.h"
#include "web_server/web_server.h"
#include "wifi_manager.h"
#include <Arduino.h>

// Function to log temperature with structured format
void logTemperature() {
  float temperature = temperatureRead(); // Get internal temperature reading
  unsigned long timestamp = millis();    // Get timestamp in milliseconds
  logInfo("Temperature: %.2f°C", timestamp, temperature);
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  setupWiFi();

  // Initialize mDNS
  setupMDNS(HOST_NAME);

  // Start web server
  setupWebServer();
}

void loop() {
  // Log temperature every second
  logTemperature();
  delay(1000); // Delay for 1 second
}
