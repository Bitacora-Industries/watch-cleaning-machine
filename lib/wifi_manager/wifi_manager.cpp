#include "wifi_manager.h"
#include "config.h"
#include "logger.h"
#include <SPIFFS.h>
#include <WiFi.h>

void readWiFiCredentials(String &ssid, String &password) {
  if (!SPIFFS.begin(true)) {
    logError("Failed to mount SPIFFS!");
    return;
  }

  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    logError("Failed to open WiFi credentials file.");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("ssid=")) {
      ssid = line.substring(5);
    } else if (line.startsWith("password=")) {
      password = line.substring(9);
    }
  }
  file.close();

  ssid.trim();
  password.trim();

  if (ssid.isEmpty() || password.isEmpty()) {
    logWarn("WiFi credentials are incomplete or missing.");
  } else {
    logInfo("WiFi credentials loaded successfully.");
  }
}

void setupWiFi() {
  String ssid, password;
  readWiFiCredentials(ssid, password);

  if (ssid.isEmpty() || password.isEmpty()) {
    logError("Cannot connect to WiFi: Missing credentials.");
    return;
  }

  logInfo("Connecting to WiFi...");

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startAttemptTime = millis();
  int retryCount = 0;
  const int maxRetries = 5;

  // Retry mechanism with exponential backoff
  while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
    unsigned long elapsedTime = millis() - startAttemptTime;

    if (elapsedTime >= WIFI_TIMEOUT_MS) {
      retryCount++;
      logWarn("WiFi connection attempt %d failed, retrying...", retryCount);
      startAttemptTime = millis();
      WiFi.begin(ssid.c_str(), password.c_str());
      delay(1000 * retryCount); // Exponential backoff
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    logInfo("Connected to WiFi!");
    logInfo("IP Address: %s", WiFi.localIP().toString().c_str());
  } else {
    logError("Failed to connect to WiFi after %d retries.", maxRetries);
  }
}
