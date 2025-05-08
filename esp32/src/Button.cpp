
#include <Button.h>
#include <device.h>
#include <esp_log.h>

#include "logging.h"

#define LABEL "Button"

auto logger = Logger(LABEL);

Button::Button(const char* id, uint8_t pin) {
  this->id = id;
  this->pin = pin;
  state = State::High;
}

bool Button::pressed() {
  if (expander->digitalRead(pin) == Low) {
    if (state == High) {
      state = Low;
      ESP_LOGI(LABEL, "%s pressed", id);
      logger.info("%s pressed", id);
    }
  } else {
    if (state == Low) {
      ESP_LOGI(LABEL, "%s released", id);
      state = High;
      return true;
    }
  }
  return false;
}
