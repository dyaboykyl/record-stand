#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "AppState.h"
#include "logging.h"
#include "utils.h"

static auto logger = Logger("Bluetooh");

#define SERVICE_UUID "Identify"     // 2de7d2f2-0000-1000-8000-00805f9b34fb
#define CHARACTERISTIC_UUID "Song"  // 0000c870-0000-1000-8000-00805f9b34fb

extern AppState appState;
static BLEServer *server = NULL;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *server) { logger.info("Connected to client"); };

  void onDisconnect(BLEServer *server) {
    logger.info("Disconneted from client. Readvertising");
    server->getAdvertising()->start();
  }
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    auto value = characteristic->getValue();
    auto length = characteristic->getLength();
    logger.info("[onWrite] size=%d: %s", length, value.c_str());
    appState.onNewIdentifyResult(value.c_str());
  }

  void onRead(BLECharacteristic *characteristic) {
    logger.info("[onRead]");
    static int readCount = 1;
    auto data = "Hello " + std::to_string(readCount++);
    characteristic->setValue((uint8_t *)data.c_str(), data.size());
  }

  void onNotify(BLECharacteristic *characteristic) { logger.info("[onNotify]"); }

  void onStatus(BLECharacteristic *characteristic, Status s, uint32_t code) {
    logger.info("[onStatus] %d");
  }
};

void initBluetooth() {
  BLEDevice::init("Record Stand");
  server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
  BLEService *service = server->createService(SERVICE_UUID);
  BLECharacteristic *characteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  characteristic->setValue("Testing");
  characteristic->setCallbacks(new CharacteristicCallbacks());
  service->start();
  BLEDevice::getAdvertising()->start();
}