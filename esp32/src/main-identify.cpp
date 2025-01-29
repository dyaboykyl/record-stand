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

#include "audio.h"
#include "controller.h"
#include "network.h"
#include "screen.h"
#include "storage.h"
#include "wav.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define TAG "Main-Identify"

#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 5
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

void setup(void) {
  esp_log_level_set("*", ESP_LOG_DEBUG);
  Serial.begin(115200);
  delay(200);

  LOG_INFO(TAG, "Beginning");
  initAll();
}

void loop() {
  if (buttonOnePressed()) {
    runWavTask();
    // // delay(2000);
    // initAudio();
    // // scanNetworks();
  }

  if (buttonTwoPressed()) {
    connectToSavedWifi();
  }

  delay(100);
}
