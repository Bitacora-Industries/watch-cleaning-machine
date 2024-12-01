#include "mdns_manager.h"
#include "logger.h"
#include <ESPmDNS.h>

void setupMDNS(const char *hostname) {
  if (!MDNS.begin(hostname)) {
    logError("Error starting mDNS");
    return;
  }
  logInfo("mDNS responder started. Access at http://%s.local", hostname);
}
