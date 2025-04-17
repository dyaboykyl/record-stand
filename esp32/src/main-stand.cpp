
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "leds.h"
#include "screen.h"

#define LABEL "Main"

void setup() { initAll(); }

// extern void ledsColorFromSerial();
void loop() {
  if (buttonOnePressed()) {
    start();
  }
  // ESP_LOGI(LABEL, "Looping");
  delay(100);
  // ledsColorFromSerial();

  if (Serial.available() > 0) {
    auto intensity = atoi(Serial.readString().c_str());
    setIntensityThreshold(intensity);
  }
}
