
#include "controller.h"

#include <Arduino.h>

#include "acr.h"
#include "audio.h"
#include "network.h"
#include "screen.h"
#include "storage.h"
#include "wav.h"

void initAll() {
  initStorage();
  initButtons();
  initAudio();
  allocateWavSpace();
}

TaskHandle_t taskHandle1;
void recordWavAndSend(bool local) {
  recordWavAtRate(16000);
  identifySongV2(getWavData(), getWavFileSize(), local);
}

void task(void* params) {
  recordWavAndSend(true);
  for (;;) delay(1000);
}

void runWavTask() { xTaskCreatePinnedToCore(task, "Task1", 10000, NULL, 1, &taskHandle1, 0); }