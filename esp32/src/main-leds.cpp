#include <esp_log.h>

#include "controller.h"
#include "device.h"
#include "leds.h"

// #define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main"

#define LED_COUNT 90

bool running = false;

void setup() {
  initLogging();
  initButtons();
  setupLeds(LED_COUNT);
  ESP_LOGI(LABEL, "Setup complete");
}

int i = 0;
auto ledFunction = ledsOneByOne;
auto oneByOne = true;
auto dynamicLeds = false;
void loop() {
  if (buttonOnePressed()) {
    running = !running;
    ESP_LOGI(LABEL, "Running: %b %d", running, i);
    if (!running) {
      turnOffLeds();
    }
  }

  if (buttonTwoPressed()) {
    if (oneByOne) {
      oneByOne = false;
      dynamicLeds = true;
    } else if (dynamicLeds) {
      dynamicLeds = false;
    } else if (!dynamicLeds) {
      oneByOne = true;
    }
  }

  delay(5);
  if (running) {
    if (oneByOne) {
      ledsOneByOne();
    } else {
      ledsAll(dynamicLeds);
    }
  }
  if (i++ == 500) {
    ESP_LOGI(LABEL, "Looping...");
    i = 0;
  }
}
