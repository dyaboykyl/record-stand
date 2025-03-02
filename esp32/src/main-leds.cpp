// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// #include <Arduino_GFX_Library.h>
// #include <AsyncHTTPRequest_Generic.h>
// #include <EasyLogger.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include <WiFi.h>
#include <esp_log.h>
// #include <lvgl.h>

#include "leds.h"
#include "screen.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main"

#define LED_COUNT 6

bool running = true;

void setup(void) {
  esp_log_level_set("*", ESP_//LOG_INFO);
  Serial.begin(115200);
  delay(200);

  ////LOG_INFO(LABEL, "Beginning");
  setupLeds(LED_COUNT);
  initButtons();
}

extern void startScan();
extern void turnOffLeds();
void loop() {
  if (buttonOnePressed()) {
    running = !running;
    ESP_LOGI(LABEL, "Running: " << running);
    if (!running) {
      turnOffLeds();
    }
  }

  // and on the backlight
  if (buttonTwoPressed()) {
    ESP.restart();
  }

  delay(5);
  ESP_LOGI(LABEL, "Looping...");
  if (running) {
    onLoopLeds();
  }
}
