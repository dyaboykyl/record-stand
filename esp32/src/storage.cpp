#include "storage.h"

#include <EEPROM.h>
#include <WString.h>

#include "logging.h"

#define VALUE_SIZE 100
#define EEPROM_SIZE (KEY_COUNT * VALUE_SIZE)

static auto logger = Logger("Storage");

void initStorage() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    // LOG_ERROR(LABEL, "failed to init EEPROM");
    return;
  }
  logger.info("Storage initialized");
}

void writeToStorage(StorageKey key, string value) {
  int addr = key * VALUE_SIZE;
  EEPROM.writeString(addr, String(value.c_str()));
  auto result = EEPROM.commit();
  logger.debug("wrote key: %d value: %s ", key, value.c_str());
}

string readFromStorage(StorageKey key) {
  int addr = key * VALUE_SIZE;
  string value = EEPROM.readString(addr).c_str();
  logger.debug("[read] key: %d, value: %s", key, value.c_str());
  return value;
}