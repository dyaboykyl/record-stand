// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Arduino_GFX_Library.h>
#include <AsyncHTTPRequest_Generic.h>
#include <EasyLogger.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <esp_log.h>
#include <lvgl.h>
#include <screen.h>

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define TAG "Main-TS"

void setup(void) {
  esp_log_level_set("*", ESP_LOG_INFO);
  Serial.begin(115200);
  delay(200);
  // while (!Serial) delay(100);

  LOG_INFO(TAG, "Beginning");
  initScreen();
  // Serial.println("Initialized!");
}

extern void startScan();
void loop() {
  if (buttonOnePressed()) {
    startScan();
  }

  if (buttonTwoPressed()) {
    // ESP.restart();
    toggleBacklight();
  }

  lv_timer_handler(); /* let the GUI do its work */
  delay(5);           /* let this time pass */
  // reactToTouch();
  // delay(1);
  // LOG_INFO(TAG, "Looping...");
}