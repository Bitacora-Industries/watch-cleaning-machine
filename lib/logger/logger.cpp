#include "logger.h"
#include <Arduino.h>
#include <stdarg.h>

// Maximum size for a formatted log message
#define LOG_BUFFER_SIZE 256

// Helper function to print log messages
void printLog(LogLevel level, const char *format, va_list args) {
  const char *levelStr = "";
  switch (level) {
  case DEBUG:
    levelStr = "DEBUG";
    break;
  case INFO:
    levelStr = "INFO";
    break;
  case WARN:
    levelStr = "WARN";
    break;
  case ERROR:
    levelStr = "ERROR";
    break;
  case FATAL:
    levelStr = "FATAL";
    break;
  }

  char buffer[LOG_BUFFER_SIZE];
  vsnprintf(buffer, LOG_BUFFER_SIZE, format, args); // Format the string
  Serial.printf("[%s][%lu ms] %s\n", levelStr, millis(), buffer);
}

// Implementations for each log level
void logDebug(const char *format, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  printLog(DEBUG, format, args);
  va_end(args);
#endif
}

void logInfo(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printLog(INFO, format, args);
  va_end(args);
}

void logWarn(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printLog(WARN, format, args);
  va_end(args);
}

void logError(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printLog(ERROR, format, args);
  va_end(args);
}

void logFatal(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printLog(FATAL, format, args);
  va_end(args);
}
