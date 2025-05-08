
#include "controller.h"

#include <Arduino.h>

#include "acr.h"
#include "audio.h"
#include "device.h"
#include "leds.h"
#include "logging.h"
#include "network.h"
#include "screen/ScreenState.h"
#include "screen/screen.h"
#include "secrets.h"
#include "storage.h"
#include "wav.h"

using namespace std;

#define LABEL "Controller"

#define LED_COUNT 16

ScreenState screenState = ScreenState();

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
  setLevel(ESP_LOG_DEBUG);

  Serial.setDebugOutput(true);

  delay(500);
  auto start = millis();
  while (!Serial && millis() - start < 1000) {
    delay(10);
  }

  Serial.println("Serial: Logging ready");
  ESP_LOGI(LABEL, "Logging ready");
}

void initState() { screenState.init(); }

void initAll() {
  initLogging();
  initStorage();
  initAudio();
  initButtons();
  initScreen();
  // allocateWavSpace();
  setupLeds(LED_COUNT);
  initState();
}

TaskHandle_t taskHandle0;
TaskHandle_t taskHandle1;

void recordWavAndSend(void* params) {
  initWifi();
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

void persistWifiCredentials(string ssid, string password) {
  writeToStorage(StorageKey::WIFI_SSID, ssid);
  writeToStorage(StorageKey::WIFI_PASSWORD, password);
}

void connectToSavedWifi(bool connect) {
  if (isWifiConnected()) {
    return;
  }

#ifdef DEV_WIFI_SSID
  connectToWifi(DEV_WIFI_SSID, DEV_WIFI_PASSWORD, connect);
  return;
#endif

  auto ssid = readFromStorage(StorageKey::WIFI_SSID);
  if (ssid.empty()) {
    return;
  }

  auto password = readFromStorage(StorageKey::WIFI_PASSWORD);
  if (password.empty()) {
    return;
  }

  connectToWifi(ssid.c_str(), password.c_str(), connect);
  if (connect) {
    persistWifiCredentials(ssid, password);
  }
}

void readAudioAndUpdateLeds() {
  auto audioData = readI2sAudio();
  updateLedsWithAudio(audioData);
}

void readAudioAndUpdateLedsTask(void* params) {
  while (true) {
    readAudioAndUpdateLeds();
  }
}

void logLoop() {
  int last = millis();
  while (true) {
    int now = millis();
    if (now - last > 100) {
      ESP_LOGI(LABEL, "Looping");
      last = now;
    }
  }
}

void start() { runTask(readAudioAndUpdateLedsTask, 1); }