#include <esp_log.h>

#include "controller.h"
#include "leds.h"
#include "screen.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main"

#define LED_COUNT 16

bool running = false;

void setup(void) {
  initLogging();
  initButtons();
  setupLeds(LED_COUNT);
  ESP_LOGI(LABEL, "Setup complete");
}

int i = 0;
auto ledFunction = ledsOneByOne;
auto oneByOne = true;
void loop() {
  if (buttonOnePressed()) {
    running = !running;
    ESP_LOGI(LABEL, "Running: %b", running);
    if (!running) {
      turnOffLeds();
    }
  }

  if (buttonTwoPressed()) {
    if (oneByOne) {
      ledFunction = ledsAll;
    } else {
      ledFunction = ledsOneByOne;
    }
    oneByOne = !oneByOne;
  }

  delay(5);
  if (running) {
    ledFunction();
  }
  if (i++ == 1000) {
    ESP_LOGI(LABEL, "Looping...");
    i = 0;
  }
}
