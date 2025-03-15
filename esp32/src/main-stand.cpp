
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "screen.h"

#define LABEL "Main"

void setup() { initAll(); }

void loop() {
  if (buttonOnePressed()) {
    start();
  }
  ESP_LOGI(LABEL, "Looping");
  delay(1000);
}
