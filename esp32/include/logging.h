#pragma once

#include <Arduino.h>

#include <cstdarg>

void setLevel(const esp_log_level_t levelToUse);
// void setLevel(const char* label, const esp_log_level_t levelToUse) {
//   esp_log_level_set(label, levelToUse);
// }

class Logger {
 public:
  Logger(const char* label);
  void warn(const char* format, ...);
  void info(const char* format, ...);
  void debug(const char* format, ...);

 private:
  const char* label;
  void log(const esp_log_level_t level, const char* format, va_list args);
};