#ifndef LOGGER_H
#define LOGGER_H

// Log levels
enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };

// Logging functions with variadic arguments
void logDebug(const char *format, ...);
void logInfo(const char *format, ...);
void logWarn(const char *format, ...);
void logError(const char *format, ...);
void logFatal(const char *format, ...);

#endif
