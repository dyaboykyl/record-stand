#pragma once

#include <Arduino.h>

#include <vector>

void initWifi();
void scanNetworks();
bool checkWifiScan();
int getWifiStatus();
bool isWifiConnected();
std::vector<String>* getNetworks();
void connectToWifi(const char* ssid, const char* password, bool connect = false);