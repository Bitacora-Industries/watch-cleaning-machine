#include "config.h"
#include "logger.h"
#include "mdns_manager.h"
#include "web_server/web_server.h"
#include "wifi_manager.h"
#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  setupWiFi();

  // Initialize mDNS
  setupMDNS(HOST_NAME);

  // Start web server
  setupWebServer();

  logInfo("System setup complete.");
}

void loop() {
  // Add periodic tasks here if needed
  delay(10); // Small delay to avoid unnecessary CPU usage
}
