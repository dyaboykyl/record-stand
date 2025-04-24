#pragma once

#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <FreeRTOS.h>
#include <WString.h>

void initAll();
void initLogging();
void start();
void persistWifiCredentials(String ssid, String password);
void connectToSavedWifi(bool connect = true);
void runWavTask();
void runTask(TaskFunction_t task, int core);
void readAudioAndUpdateLeds();