// SPDX-FileCopyrightText: 2023 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Arduino_GFX_Library.h>
#include <AsyncHTTPRequest_Generic.h>
// #include <EasyLogger.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
// #include <esp_log.h>
#include <lvgl.h>

#include "controller.h"
#include "screen.h"
#include "storage.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main-TS"

void setup(void) {
  // auto mem = heap_caps_malloc(LV_MEM_SIZE, MALLOC_CAP_SPIRAM);
  initLogging();

  ESP_LOGI(LABEL, "Beginning");
  initStorage();
  initButtons();
  initScreen();
  ESP_LOGI(LABEL, "Initialized");
  // ESP_LOGI(LABEL, "LV MEM LOCATION: %p", mem);
}

extern void startScan();
extern void checkWifi();
int i = 0;
void loop() {
  if (buttonOnePressed()) {
    startScan();
    // writeToStorage(WIFI_SSID, "test ssid");
    // auto ssid = readFromStorage(WIFI_SSID);
  }

  if (buttonTwoPressed()) {
    // ESP.restart();
    // checkWifi();
    rotateScreen();
  }

  lv_timer_handler_run_in_period(5); /* let the GUI do its work */
  delay(5);                          /* let this time pass */
  // reactToTouch();
  // delay(1);
  if (i++ == 1000) {
    ESP_LOGI(LABEL, "Looping...");
    i = 0;
  }
}
