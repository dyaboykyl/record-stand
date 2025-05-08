
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "device.h"
#include "leds.h"
#include "screen.h"
#include "screen/ScreenState.h"
#include "screen/calibration_screen.h"
#include "screen/main_screen.h"

void setup() {
  initAll();
  startScreen();
}

void loop() {
  delay(5);
  screenLoop();

  if (button1.pressed()) {
    screenState.goToScreen(Screen::CALIBRATION);
  }
  if (button2.pressed()) {
    screenState.goToScreen(Screen::MAIN);
  }
}
