#pragma once

#include <Arduino.h>

#include <vector>

void setupWifi();
void scanNetworks();
bool checkWifiScan();
int getWifiStatus();
std::vector<String>* getNetworks();
void connectToWifi(const char* ssid, const char* password, bool connect = false);