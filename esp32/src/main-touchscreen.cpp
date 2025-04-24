

#include <Arduino_GFX_Library.h>
#include <AsyncHTTPRequest_Generic.h>
#include <SPI.h>
#include <WiFi.h>
#include <lvgl.h>

#include "controller.h"
#include "device.h"
#include "screen.h"
#include "storage.h"
#include "leds.h"
#include "audio.h"

#define LOG_LEVEL LOG_LEVEL_NOTICE
#define LABEL "Main-TS"

void setup(void) {
  // auto mem = heap_caps_malloc(LV_MEM_SIZE, MALLOC_CAP_SPIRAM);
  initAll();
  ESP_LOGI(LABEL, "Initialized");
  // ESP_LOGI(LABEL, "LV MEM LOCATION: %p", mem);
}

extern void startScan();
extern void checkWifi();
int i = 0;
bool plotting = false;
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

  // loopLvgl();
  if(plotting) {
    // updateLedsWithAudio(readAnalogAudio());
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
