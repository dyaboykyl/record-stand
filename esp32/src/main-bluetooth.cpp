
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "bluetooth.h"
#include "controller.h"
#include "device.h"
#include "logging.h"
#include "screen.h"

static auto logger = Logger("Main");

void setup() {
  initAll();

  logger.info("Ready");
}

void loop() {
  if (button1.pressed()) {
    logger.info("Button 1 pressed");
    appState.onNewIdentifyResult("{\"title\":\"ABC\",\"artist\":\"The Beatles\",\"album\":\"\"}");
  }

  if (button2.pressed()) {
  }

  delay(20);
}
