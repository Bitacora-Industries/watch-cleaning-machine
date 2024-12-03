#ifndef CONFIG_H
#define CONFIG_H

// Configuration constants
#define HOST_NAME "bitacora"
#define BASE_URL "http://bitacora.local/"
#define WIFI_TIMEOUT_MS 20000 // Timeout in milliseconds

#define STEP_PIN 27
#define DIR_PIN 14

// Ensure HOST_NAME is defined
#ifndef HOST_NAME
#error "HOST_NAME is not defined in config.h"
#endif

#endif
