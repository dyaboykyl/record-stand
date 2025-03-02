
#include <Button.h>
// #include <EasyLogger.h>
#include <screen.h>

#define LABEL "Button"

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
