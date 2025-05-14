
#include <Arduino.h>

#include "AppState.h"
#include "controller.h"
#include "device.h"
#include "leds.h"
#include "screen.h"
#include "screen/main_screen.h"

void setup() {
  initAll();
  // delay(300);
  // startScreen();
}

void loop() {
  delay(5);
  // screenLoop();

  static int i = 1;
  if (button1.pressed()) {
    // appState.onNewIdentifyResult(
    //     (string("Song ") + to_string(i++) + string("+=+The Beatles+=+NO_RESULT+=+NO_RESULT"))
    //         .c_str());
  }
  if (button2.pressed()) {
    // appState.goToScreen(Screen::SETTINGS);
  }
}
