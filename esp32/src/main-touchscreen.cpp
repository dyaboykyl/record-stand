

#include <Arduino_GFX_Library.h>
#include <AsyncHTTPRequest_Generic.h>
#include <SPI.h>
#include <WiFi.h>
#include <lvgl.h>

#include "audio.h"
#include "controller.h"
#include "device.h"
#include "leds.h"
#include "screen.h"
#include "storage.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main-TS"

void setup(void) {
  // auto mem = heap_caps_malloc(LV_MEM_SIZE, MALLOC_CAP_SPIRAM);
  initAll();
  ESP_LOGI(LABEL, "Initialized");
  void* lvglSpace = heap_caps_malloc(LV_MEM_SIZE, MALLOC_CAP_SPIRAM);
  ESP_LOGI(LABEL, "LV_MEM_ADR: %p", lvglSpace);
  // ESP_LOGI(LABEL, "LV MEM LOCATION: %p", mem);
}

extern void startScan();
extern void checkWifi();
int i = 0;
bool plotting = true;
void loop() {
  if (button1.pressed()) {
    // startScan();
    // writeToStorage(WIFI_SSID, "test ssid");
    // auto ssid = readFromStorage(WIFI_SSID);
    plotting = !plotting;
  }

  if (button2.pressed()) {
    // ESP.restart();
    // checkWifi();
    // rotateScreen();
  }

  loopLvgl();
  if (plotting) {
    // updateLedsWithAudio(readAnalogAudio());
    ledsAll(true);
    // plotAudio();
  }
  // reactToTouch();
  // delay(1);
  if (i++ == 10000) {
    // ESP_LOGI(LABEL, "Loping...");
    i = 0;
  }

  if (Serial.available() > 0) {
    auto data = Serial.readString();
    ESP_LOGI(LABEL, "Received data: %s", data.c_str());
    if (data == "start") {
      initI2s();
    } else if (data == "stop") {
      stopI2s();
    }
  }
}
