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

void connect() {
  auto ssid = readFromStorage(WIFI_SSID);
  if (!ssid || ssid == "") {
    return;
  }

  auto password = readFromStorage(WIFI_PASSWORD);
  if (!password || password == "") {
    return;
  }

  connectToWifi(ssid.c_str(), password.c_str(), true);
}

void setup(void) {
  esp_log_level_set("*", ESP_LOG_INFO);
  Serial.begin(115200);
  delay(200);

  LOG_INFO(TAG, "Beginning");
  initAll();
}

void loop() {
  if (buttonOnePressed()) {
    runWavTask();
  }

  if (buttonTwoPressed()) {
    connect();
  }

  delay(100);
}
