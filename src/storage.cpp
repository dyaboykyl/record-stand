#include "storage.h"

#include <EEPROM.h>
#include <EasyLogger.h>
#include <WString.h>

#define VALUE_SIZE 100
#define EEPROM_SIZE (KEY_COUNT * VALUE_SIZE)

#define TAG "Storage"

void setupStorage() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to init EEPROM");
    return;
  }
}

void writeToStorage(StorageKey key, String value) {
  int addr = key * VALUE_SIZE;
  EEPROM.writeString(addr, value);
  auto result = EEPROM.commit();
  LOG_DEBUG(TAG, (String) "wrote " + key + ": " + value + " result: " + result);
}

String readFromStorage(StorageKey key) {
  int addr = key * VALUE_SIZE;
  String value = EEPROM.readString(addr);
  LOG_DEBUG(TAG, (String) "read " + key + ": " + value);
  return value;
}