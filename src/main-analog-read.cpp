
#include <Arduino.h>
#include <EasyLogger.h>
#include <driver/adc.h>

#include "audio.h"
#include "controller.h"
#include "screen.h"
#include "storage.h"

#define TAG "Main"
#define ADC_CHANNEL ADC2_CHANNEL_5

void setup() {
  esp_log_level_set("*", ESP_LOG_INFO);
  Serial.begin(115200);
  delay(1000);
  // adc1_config_width(ADC_WIDTH_BIT_12);
  // adc2_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

  initStorage();
  initButtons();
  initScreen();
  LOG_INFO(TAG, (String) "[APP] Free memory bytes: " + esp_get_free_heap_size());
}

struct Filter {
  float alpha = 0;
  short prev = 0;
  short avg = 0;
  Filter(float a) : alpha(a), avg(0) {}
};

void runFilter(Filter filter, short value) {
  filter.avg = filter.avg * filter.alpha + (1 - filter.alpha) * value;
  Serial.print((String) ">" + filter.alpha + ":");
  Serial.println(value - filter.avg);
}

void runLowPass(Filter filter, short value) {
  filter.prev = filter.alpha * value + (1 - filter.alpha) * filter.prev;
  Serial.print((String) ">" + filter.alpha + ":");
  Serial.println(filter.prev);
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
  if (buttonOnePressed()) {
    initAudio();
    // LOG_INFO(TAG, (String) "Reading: " + reading);
    // reading = !reading;
  }

  if (buttonTwoPressed()) {
    initScreen();
  }

  if (reading) {
    // value = readAudio(false, false);
    // for (int i = 0; i < 5; i++) {
    //   // runLowPass(filters[i], value);
    // }

    auto audioData = readI2sAudio();
    sampleCount += audioData->size;
    if (millis() - last >= 1000) {
      samplesPerSecond = sampleCount;
      sampleCount = 0;
      last = millis();
    }
    // for (int i = 0; i < audioData->size; i++) {
    //   Serial.print(">Raw:");
    //   Serial.println(audioData->samples[i]);
    //   Serial.print(">Size:");
    //   Serial.println(audioData->size);
    // }
    Serial.print(">Sample Rate:");
    Serial.println(samplesPerSecond);
  }
}
