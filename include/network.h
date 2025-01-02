#pragma once

#include <Arduino.h>

#include <vector>

void setupWifi();
void scanNetworks();
bool checkWifiScan();
std::vector<String> *getNetworks();