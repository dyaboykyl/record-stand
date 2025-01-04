
#pragma once

#include <WString.h>

enum StorageKey {
  WIFI_SSID,
  WIFI_PASSWORD,

  KEY_COUNT
};

void setupStorage();
void writeToStorage(StorageKey key, String value);
String readFromStorage(StorageKey key);