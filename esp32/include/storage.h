
#pragma once

#include <WString.h>

enum StorageKey {
  WIFI_SSID_KEY,
  WIFI_PASSWORD_KEY,

  KEY_COUNT
};

void initStorage();
void writeToStorage(StorageKey key, String value);
String readFromStorage(StorageKey key);