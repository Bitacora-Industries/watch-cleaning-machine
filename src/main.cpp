#include "SPIFFS.h"
#include "config.h"
#include "logger.h"
#include "mdns_manager.h"
#include "web_server/web_server.h"
#include "wifi_manager.h"
#include <Arduino.h>

void setupSPIFFS() {
  if (!SPIFFS.begin(true)) {
    logError("Failed to mount SPIFFS!");
  } else {
    logInfo("SPIFFS mounted successfully.");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS
  setupSPIFFS();

  // Initialize WiFi
  setupWiFi();

  // Initialize mDNS (Too slow, for now)
  setupMDNS(HOST_NAME);

  // Start web server
  setupWebServer();

  logInfo("System setup complete.");
}
