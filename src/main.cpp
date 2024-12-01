#include <Arduino.h>
#include "wifi_manager.h"
#include "web_server.h"
#include "mdns_manager.h"
#include "config.h"

// Function to log temperature with structured format
void logTemperature() {
  float temperature = temperatureRead();  // Get internal temperature reading
  unsigned long timestamp = millis();     // Get timestamp in milliseconds
  Serial.printf("[INFO][%lu ms] Temperature: %.2f°C\n", timestamp, temperature);
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
  delay(1000);  // Delay for 1 second
}
