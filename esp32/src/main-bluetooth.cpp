
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
// #include <EasyLogger.h>

#include "controller.h"
#include "screen.h"

#define LABEL "Main"

#define SERVICE_UUID "Identify"     // 2de7d2f2-0000-1000-8000-00805f9b34fb
#define CHARACTERISTIC_UUID "Song"  // 0000c870-0000-1000-8000-00805f9b34fb

BLEServer *pServer = NULL;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { ESP_LOGI(LABEL, "Connected to client"); };

  void onDisconnect(BLEServer *pServer) {
    ESP_LOGI(LABEL, "Disconneted from client");
    ESP_LOGI(LABEL, ">> Readvertising");
    pServer->getAdvertising()->start();
  }
};

int readCount = 1;

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    auto value = pCharacteristic->getValue();
    auto length = pCharacteristic->getLength();
    ESP_LOGI(LABEL, "Received value from client: " << length);
    ESP_LOGI(LABEL, value.c_str());
  }

  void onRead(BLECharacteristic *pCharacteristic) {
    ESP_LOGI(LABEL, "Read request");
    auto data = "Hello " + std::to_string(readCount++);
    pCharacteristic->setValue((uint8_t *)data.c_str(), data.size());
  }

  void onNotify(BLECharacteristic *pCharacteristic) { ESP_LOGI(LABEL, "Notify"); }

  void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) {
    ESP_LOGI(LABEL, "Status " << s);
  }
};

void setup() {
  initLogging();
  initButtons();

  BLEDevice::init("Record Stand");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Testing");
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();

  ESP_LOGI(LABEL, "Ready");
}

void loop() {
  if (buttonOnePressed()) {
  }

  if (buttonTwoPressed()) {
  }

  delay(20);
}
