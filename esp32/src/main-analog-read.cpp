
#include <Arduino.h>
#include <driver/adc.h>

#include "audio.h"
#include "controller.h"
#include "screen.h"
#include "storage.h"
#include "device.h"

#define LABEL "Main"
#define ADC_CHANNEL ADC2_CHANNEL_5

void setup() {
  initAll();
  ESP_LOGI(LABEL, "[APP] Free memory bytes: %d", esp_get_free_heap_size());
}

struct Filter {
  float alpha = 0;
  short prev = 0;
  short avg = 0;
  Filter(float a) : alpha(a), avg(0) {}
};

void runFilter(Filter filter, short value) {
  filter.avg = filter.avg * filter.alpha + (1 - filter.alpha) * value;
  // Serial.print(">" + filter.alpha + ":");
  Serial.println(value - filter.avg);
}

Filter filters[] = {
    Filter(.01), Filter(.99), Filter(.75), Filter(.25), Filter(.05),
};

short value = 0;
bool reading = false;
int16_t mapInt32ToInt16(int32_t value) {
  // Scale the value from int32_t range to int16_t range
  return static_cast<int16_t>(
      (value - INT32_MIN) * (INT16_MAX - INT16_MIN) / (INT32_MAX - INT32_MIN) + INT16_MIN);
}

auto last = millis();
int samplesPerSecond = 0;
int sampleCount = 0;
void loop() {
  if (button1.pressed()) {
    ESP_LOGI(LABEL, "Reading: %d", reading);
    reading = !reading;
  }

  if (button2.pressed()) {
    // connectToSavedWifi();

    int32_t int32 = (-0x7ab290 << 8) + 2656;
    Serial.print("int32 ");
    Serial.print(int32);
    Serial.print(": ");
    Serial.println(int32, 2);

    int32_t int24 = int32 >> 8;
    Serial.print("int24 ");
    Serial.print(int24);
    Serial.print(": ");
    Serial.println(int24, 2);

    int16_t int24Scaled16 = ((float)int24 / 0x7fffff) * INT16_MAX;
    Serial.print("int24Scaled16 ");
    Serial.print(int24Scaled16);
    Serial.print(": ");
    Serial.println(int24Scaled16, 2);

    int16_t int24Mapped16 = map(int24, -0x7fffff - 1, 0x7fffff, INT16_MIN, INT16_MAX);
    Serial.print("int24Shifted8Mapped16 ");
    Serial.print(int24Mapped16);
    Serial.print(": ");
    Serial.println(int24Mapped16, 2);

    Serial.print("int24 percent ");
    Serial.println((double)int24 / 0x7fffff);

    Serial.print("int24Scaled16 percent ");
    Serial.println((double)int24Scaled16 / INT16_MAX);

    Serial.print("int24Mapped16 percent ");
    Serial.println((double)int24Mapped16 / INT16_MAX);
  }

  if (reading) {
    // value = readAudio(false, false);
    // for (int i = 0; i < 5; i++) {
    //   // runLowPass(filters[i], value);
    // }

    // auto audioData = readI2sAudio();
    // sampleCount += audioData->size;
    sampleCount++;
    if (millis() - last >= 1000) {
      samplesPerSecond = sampleCount;
      sampleCount = 0;
      last = millis();
    }
    // for (int i = 0; i < audioData->size; i++) {
    //   Serial.print(">Raw:");
    //   Serial.println(audioData->samples[i]);
    //   //   Serial.print(">Size:");
    //   //   Serial.println(audioData->size);
    // }
    Serial.print(">Mic:");
    Serial.println(analogRead(A1));
    Serial.print(">Sample Rate:");
    Serial.println(samplesPerSecond);
  }
}
