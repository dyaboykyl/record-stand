
#define LOGGING true

#include <Arduino.h>

#include "controller.h"
#include "device.h"
#include "leds.h"
#include "screen.h"
#include "screen/ScreenController.h"
#include "screen/calibration_screen.h"

#define LABEL "Main"

auto screenController = ScreenController();

void runControllers() {
  ESP_LOGI(LABEL, "Running Controllers...");
  screenController.init();
  // screenController.screenOffset.set({0, 4});
  ESP_LOGI(LABEL, "Done running Controllers...");
}

void setup() {
  initAll();
  runControllers();
  startScreen();
}

void loop() {
  delay(5);
  screenLoop();

  if (button1.pressed()) {
    ESP_LOGI(LABEL, "Button 1 pressed");
    runControllers();
  }
}
