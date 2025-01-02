
#include <Button.h>
#include <screen.h>

Button::Button(uint8_t pin) {
  this->pin = pin;
  state = State::Low;
}

bool Button::pressed() {
    if (expander->digitalRead(pin) == Low) {
    if (state == High) {
      state = Low;
      return true;
    }
  } else {
    state = High;
  }
  return false;
}
