
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <EasyLogger.h>

#include "controller.h"
#include "screen.h"

#define LABEL "Main"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer = NULL;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { LOG_INFO(LABEL, "Connected to client"); };

  void onDisconnect(BLEServer *pServer) {
    LOG_INFO(LABEL, "Disconneted from client");
    LOG_INFO(LABEL, ">> Readvertising");
    pServer->getAdvertising()->start();
  }
};

int readCount = 1;

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    auto value = pCharacteristic->getValue();
    auto length = pCharacteristic->getLength();
    LOG_INFO(LABEL, "Received value from client: " << length);
    LOG_INFO(LABEL, value.c_str());
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    LOG_INFO(LABEL, "Read request");
    auto data = "Hello " + std::to_string(readCount++);
    pCharacteristic->setValue((uint8_t *)data.c_str(), data.size());
  }

  void onNotify(BLECharacteristic *pCharacteristic) { LOG_INFO(LABEL, "Notify"); }

  void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) {
    LOG_INFO(LABEL, "Status " << s);
  }
};

void setup() {
  initLogging();
  initButtons();

  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Testing");
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();

  LOG_INFO(LABEL, "Ready");
}

void loop() {
  if (buttonOnePressed()) {
  }

  if (buttonTwoPressed()) {
  }

  delay(20);
}
