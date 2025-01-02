// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// #include <Arduino_GFX_Library.h>
// #include <AsyncHTTPRequest_Generic.h>
#include <EasyLogger.h>
// #include <SPI.h>
// #include <TFT_eSPI.h>
// #include <WiFi.h>
#include <esp_log.h>
// #include <lvgl.h>

#include "leds.h"
#include "screen.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define TAG "Main"

#define LED_COUNT 6

bool running = true;

void setup(void) {
  esp_log_level_set("*", ESP_LOG_INFO);
  Serial.begin(115200);
  delay(200);

  LOG_INFO(TAG, "Beginning");
  setupLeds(LED_COUNT);
  initButtons();
}

extern void startScan();
extern void turnOffLeds();
void loop() {
  if (buttonOnePressed()) {
    running = !running;
    LOG_INFO(TAG, (String) "Running: " + running);
    if (!running) {
      turnOffLeds();
    }
  }

  // and on the backlight
  if (buttonTwoPressed()) {
    ESP.restart();
  }

  delay(5);
  // LOG_INFO(TAG, "Looping...");
  if (running) {
    onLoopLeds();
  }
}
