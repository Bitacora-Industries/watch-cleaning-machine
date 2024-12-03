#include "web_server.h"
#include "SPIFFS.h"
#include "config.h"
#include "logger.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// Create an instance of the AsyncWebServer
AsyncWebServer server(80);

// Flag to track whether the motor is ON or OFF
volatile bool isMotorOn = false;

// Flag to track the motor direction:
// false = clockwise, true = counterclockwise
volatile bool motorDirection = false;

volatile int motorSpeed = 750; // Default speed (delay in microseconds)

// Motor behavior parameters
volatile bool isCleaning = false;
volatile bool isRinsing = false;
volatile bool isDrying = false;

// Motor control function
void handleMotorBehavior(String mode) {
  logInfo("Starting cleaning cycle...");
  if (mode == "cleaning") {
    isCleaning = true;
    isRinsing = false;
    isDrying = false;
    motorSpeed =
        200; // Adjust delay for ~200-300 RPM (calculated based on motor specs)
  } else if (mode == "rinsing") {
    isCleaning = false;
    isRinsing = true;
    isDrying = false;
    motorSpeed = 200; // Adjust delay for ~200-300 RPM
  } else if (mode == "drying") {
    isCleaning = false;
    isRinsing = false;
    isDrying = true;
    motorSpeed = 100; // Adjust delay for max RPM
  }

  isMotorOn = true;
  motorDirection = false; // Start in clockwise direction
}

void initializeRoutes() {
  // Static files
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.serveStatic("/script.js", SPIFFS, "/script.js");
  server.serveStatic("/style.css", SPIFFS, "/style.css");

  // Cleaning cycle
  server.on("/cleaning", HTTP_POST, [](AsyncWebServerRequest *request) {
    handleMotorBehavior("cleaning");
    request->send(200, "text/plain", "Motor set to Cleaning mode");
  });

  // Rinsing cycle
  server.on("/rinsing", HTTP_POST, [](AsyncWebServerRequest *request) {
    handleMotorBehavior("rinsing");
    request->send(200, "text/plain", "Motor set to Rinsing mode");
  });

  // Drying cycle
  server.on("/drying", HTTP_POST, [](AsyncWebServerRequest *request) {
    handleMotorBehavior("drying");
    request->send(200, "text/plain", "Motor set to Drying mode");
  });

  // Emergency Stop Route
  server.on("/emergency-stop", HTTP_POST, [](AsyncWebServerRequest *request) {
    isMotorOn = false;       // Turn off the motor
    isCleaning = false;
    isRinsing = false;
    isDrying = false;
    logError("Emergency stop activated! Motor stopped.");
    digitalWrite(STEP_PIN, LOW); // Ensure STEP_PIN is LOW
    request->send(200, "text/plain", "Emergency stop activated!");
  });

  logInfo("Routes initialized.");
}

void initializeMotor() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
}

void setupWebServer() {
  // Initialize motor pins
  initializeMotor();

  // Initialize routes and start the server
  initializeRoutes();
  server.begin();
  logInfo("Web server initialized on port 80.");
}

// Main loop to control motor stepping
void loop() {
  static unsigned long lastStepTime = 0;              // Tracks the last step time
  static unsigned long lastDirectionChangeTime = 0;   // Tracks the last direction change
  static unsigned long stopStartTime = 0;             // Tracks the stop moment
  static bool stopping = false;                       // Flag to indicate a stop is in progress

  const unsigned int pulseWidth = 10;                 // Short pulse width for TMC2209
  const unsigned int stopDuration = 500;             // Stop duration in milliseconds
  const unsigned int directionChangeInterval = 5000; // Direction change interval in milliseconds

  unsigned long now = millis();

  // Check if motor needs to stop momentarily before switching direction
  if ((isCleaning || isRinsing) && now - lastDirectionChangeTime >= directionChangeInterval) {
    if (!stopping) {
      stopping = true;                // Initiate stopping
      stopStartTime = now;            // Record the stop start time
      digitalWrite(STEP_PIN, LOW);    // Ensure STEP_PIN is LOW
      logInfo("Pausing motor before direction change...");
    } else if (now - stopStartTime >= stopDuration) {
      motorDirection = !motorDirection; // Switch direction
      digitalWrite(DIR_PIN, motorDirection ? HIGH : LOW);
      lastDirectionChangeTime = now;    // Record the last direction change time
      stopping = false;                 // Reset stopping flag
      logInfo("Direction changed to %s", motorDirection ? "Counterclockwise" : "Clockwise");
    }
    return; // Skip the rest of the loop during the stop period
  }

  // Generate step pulses if the motor is ON
  if (isMotorOn && !stopping) {
    unsigned long stepNow = micros();
    if (stepNow - lastStepTime >= motorSpeed) { // Delay between steps
      lastStepTime = stepNow;

      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(pulseWidth); // Short pulse width
      digitalWrite(STEP_PIN, LOW);
    }
  }
}
