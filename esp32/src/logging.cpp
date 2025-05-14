
#include "logging.h"

#include <Arduino.h>

#include <cstdarg>

void setLevel(const esp_log_level_t levelToUse) { esp_log_level_set("*", levelToUse); }
// void setLevel(const char* label, const esp_log_level_t levelToUse) {
//   esp_log_level_set(label, levelToUse);
// }
Logger::Logger(const char* label) : label(label) {};

void Logger::warn(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(ESP_LOG_WARN, format, args);
  va_end(args);
}

void Logger::info(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(ESP_LOG_INFO, format, args);
  va_end(args);
}

void Logger::debug(const char* format, ...) {
  va_list args;
  va_start(args, format);
  log(ESP_LOG_DEBUG, format, args);
  va_end(args);
}

void Logger::log(const esp_log_level_t level, const char* format, va_list args) {
  if (level > esp_log_level_get(label)) {
    return;
  }

  Serial.print("[");
  Serial.print(level);
  Serial.print("] (");
  Serial.print(label);
  Serial.print(") ");
  char buffer[256];
  vsnprintf(buffer, sizeof(buffer), format, args);
  Serial.print(buffer);
  Serial.println();
}
