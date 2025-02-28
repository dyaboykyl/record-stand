
#include <Button.h>
#include <EasyLogger.h>
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
      LOG_INFO(LABEL, String(id) << " pressed");
    }
  } else {
    if (state == Low) {
      LOG_INFO(LABEL, String(id) << " released");
      state = High;
      return true;
    }
  }
  return false;
}
