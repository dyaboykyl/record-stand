#pragma once

#include <FreeRTOS.h>
#include <WString.h>

void initAll();
void initLogging();
void persistWifiCredentials(String ssid, String password);
void connectToSavedWifi(bool connect = true);
void runWavTask();
void runTask(TaskFunction_t task, int core);
