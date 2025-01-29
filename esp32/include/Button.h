
#pragma once

#include <stdint.h>

enum State : uint8_t {
  Low = 0,
  High = 1,
};

class Button {
 private:
  const char* id;
  uint8_t pin;

 public:
  State state;
  Button(const char* id, uint8_t pin);
  bool pressed();
};
