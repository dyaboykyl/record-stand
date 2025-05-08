
#pragma once

#include <string>

using namespace std;

enum StorageKey {
  WIFI_SSID,
  WIFI_PASSWORD,

  SCREEN_OFFSET,

  KEY_COUNT
};

void initStorage();
void writeToStorage(StorageKey key, string value);
string readFromStorage(StorageKey key);