
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <EasyLogger.h>

#include "controller.h"
#include "screen.h"

#define TAG "Main"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer = NULL;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { LOG_INFO(TAG, "Connected to client"); };

  void onDisconnect(BLEServer *pServer) {
    LOG_INFO(TAG, "Disconneted from client");
    LOG_INFO(TAG, ">> Readvertising");
    pServer->getAdvertising()->start();
  }
};

int readCount = 1;

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    LOG_INFO(TAG, "Received value from client: ");
    LOG_INFO(TAG, rxValue.c_str());
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    LOG_INFO(TAG, "Read request");
    auto data = "Hello " + std::to_string(readCount++);
    pCharacteristic->setValue((uint8_t *)data.c_str(), data.size());
  }

  void onNotify(BLECharacteristic *pCharacteristic) { LOG_INFO(TAG, "Notify"); }

  void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) {
    LOG_INFO(TAG, (String) "Status " + s);
  }
};

void setup() {
  esp_log_level_set("*", ESP_LOG_DEBUG);
  Serial.begin(115200);
  delay(500);
  initButtons();

  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Testing");
  pCharacteristic->setValue(std::string(String("Hello " + readCount++).c_str()));
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();

  LOG_INFO(TAG, "Ready");
}

void loop() {
  if (buttonOnePressed()) {
  }

  if (buttonTwoPressed()) {
  }

  delay(20);
}
