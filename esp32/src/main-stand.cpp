
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "device.h"
#include "leds.h"
#include "screen.h"
#include "screen/ScreenState.h"
#include "screen/calibration_screen.h"

#define LABEL "Main"

void setup() {
  initAll();
  startScreen();
}

void loop() {
  delay(5);
  screenLoop();

  if (button1.pressed()) {
    ESP_LOGI(LABEL, "Button 1 pressed");
  }
}
