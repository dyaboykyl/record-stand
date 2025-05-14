#pragma once

#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <FreeRTOS.h>
#include <WString.h>

#include "AppState.h"

extern AppState appState;
extern QueueHandle_t queueHandle;

struct BleMessage {
  size_t length;
  char data[512];  // Adjust size as needed (max BLE packet size is 512, usually 20–100 bytes)
};

void initAll();
void processBleQueue(BleMessage &message);
void initLogging();
void start();
void persistWifiCredentials(String ssid, String password);
void connectToSavedWifi(bool connect = true);
void runWavTask();
void runTask(TaskFunction_t task, int core);
void readAudioAndUpdateLeds();