
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "leds.h"
#include "screen.h"

#define LABEL "Main"

void setup() { initAll(); }

void loop() {
  if (buttonOnePressed()) {
    start();
  }
  // ESP_LOGI(LABEL, "Looping");
  delay(100);

  if (Serial.available() > 0) {
    auto intensity = atoi(Serial.readString().c_str());
    setIntensityThreshold(intensity);
  }
}
