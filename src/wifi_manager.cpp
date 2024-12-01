#include <WiFi.h>
#include <SPIFFS.h>
#include "wifi_manager.h"
#include "config.h"

void readWiFiCredentials(String &ssid, String &password) {
  if (!SPIFFS.begin(true)) {
    Serial.println("[ERROR] Failed to mount SPIFFS!");
    return;
  }

  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    Serial.println("[ERROR] Failed to open WiFi credentials file.");
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
}

void setupWiFi() {
  String ssid, password;
  readWiFiCredentials(ssid, password);

  Serial.println("[INFO] Connecting to WiFi...");

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startAttemptTime = millis();

  // Wait for connection or timeout
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS) {
    Serial.print(".");
    delay(500);  // Delay to avoid spamming logs
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[INFO] Connected to WiFi!");
    Serial.printf("[INFO] IP Address: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[ERROR] Failed to connect to WiFi.");
  }
}
