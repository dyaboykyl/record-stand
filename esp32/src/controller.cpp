
#include "controller.h"

#include <Arduino.h>
// #include <EasyLogger.h>

#include "acr.h"
#include "audio.h"
#include "network.h"
#include "screen.h"
#include "secrets.h"
#include "storage.h"
#include "wav.h"

#define LABEL "Controller"

int vprintfSerial(const char* fmt, va_list args) {
  char log_print_buffer[256];
  vsprintf(log_print_buffer, fmt, args);

  Serial.print(log_print_buffer);

  return 0;
}

void initLogging() {
  Wire.begin();
  Serial.begin(115200);

  esp_log_set_vprintf(vprintfSerial);
  esp_log_level_set("*", ESP_LOG_DEBUG);

  Serial.setDebugOutput(true);

  auto start = millis();
  while (!Serial || millis() - start < 500) {
    delay(10);
  }

  ESP_LOGI(LABEL, "Logging ready");
}

void initAll() {
  initLogging();
  initStorage();
  initButtons();
  initScreen();
  allocateWavSpace();
  initAudio();
  // initWifi();
}

TaskHandle_t taskHandle0;
TaskHandle_t taskHandle1;

// void recordWavAndSend(bool local) {
//   recordWavAtRate(16000);
//   identifySongV2(getWavData(), getWavFileSize(), local);
// }

void recordWavAndSend(void* params) {
  initWifi();
  // recordWavAtRate(16000);
  recordWavFromI2S();
  connectToSavedWifi();
  identifySongV2(getWavData(), getWavFileSize(), true);
  for (;;) delay(1000);
}

void runTask(TaskFunction_t task, int core) {
  xTaskCreatePinnedToCore(task, "Task1", 10000, NULL, 1, core == 0 ? &taskHandle0 : &taskHandle1,
                          core);
}

void runWavTask() { runTask(recordWavAndSend, 1); }

void persistWifiCredentials(String ssid, String password) {
  writeToStorage(WIFI_SSID_KEY, ssid);
  writeToStorage(WIFI_PASSWORD_KEY, password);
}

void connectToSavedWifi(bool connect) {
  if (isWifiConnected()) {
    return;
  }

#ifdef WIFI_SSID
  connectToWifi(WIFI_SSID, WIFI_PASSWORD, connect);
  return;
#endif

  String ssid = readFromStorage(WIFI_SSID_KEY);
  if (!ssid || ssid == "") {
    return;
  }

  String password = readFromStorage(WIFI_PASSWORD_KEY);
  if (!password || password == "") {
    return;
  }

  connectToWifi(ssid.c_str(), password.c_str(), connect);
  if (connect) {
    persistWifiCredentials(ssid, password);
  }
}