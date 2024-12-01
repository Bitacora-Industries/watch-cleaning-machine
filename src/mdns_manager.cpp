#include <Arduino.h>
#include <ESPmDNS.h>
#include "mdns_manager.h"

void setupMDNS(const char* hostname) {
  if (!MDNS.begin(hostname)) {
    Serial.println("Error starting mDNS");
    return;
  }
  Serial.printf("mDNS responder started. Access at http://%s.local\n", hostname);
}
