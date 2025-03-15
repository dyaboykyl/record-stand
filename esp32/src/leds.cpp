
#include "leds.h"

// #include <EasyLogger.h>
#include <FastLED.h>

#define LABEL "Leds"

#define BRIGHTNESS 255
#define LED_PIN MISO
#define MAX_LEDS 150

int ledCount = 16;

CRGB leds[MAX_LEDS];
unsigned int ledUpdateMillis = 0;

void turnOffLeds() {
  ESP_LOGI(LABEL, "Turning off leds");
  for (int i = 0; i < MAX_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

CRGB scroll(int pos) {
  CRGB color(0, 0, 0);
  float threshold1 = ledCount / 3.0f;
  float threshold2 = threshold1 * 2;

  if (pos < threshold1) {
    color.g = 0;
    color.r = ((float)pos / threshold1) * 170;
    color.b = 255 - color.r;
  } else if (pos < threshold2) {
    color.g = ((float)(pos - threshold1) / threshold1) * 170;
    color.r = 170 - color.g;
    color.b = 0;
  } else if (pos <= ledCount) {
    color.b = ((float)(pos - threshold2) / threshold1) * 255;
    color.g = 170 - color.b;
    color.r = 1;
  }
  return color;
}

void setupLeds(int totalLeds) {
  ledCount = totalLeds;
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, ledCount).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  turnOffLeds();
  ESP_LOGI(LABEL, "Setup complete");
}

void ledsOneByOne() {
  static int i = 0;
  if ((millis() - ledUpdateMillis) > 50) {
    for (int i = 0; i < ledCount; i++) {
      leds[i] = CRGB::Black;
    }
    ESP_LOGD(LABEL, "Updating at i %d", i);
    ledUpdateMillis = millis();
    leds[i] = scroll((i) % ledCount);
    i = (i + 1) % ledCount;
    FastLED.show();
  }
}

void ledsAll() {
  static int offset = 0;
  if (Serial.available() > 0) {
    offset = Serial.readString().toInt();
    ESP_LOGI(LABEL, "Received data: %d", offset);
  }
  if ((millis() - ledUpdateMillis) > 250) {
    for (int i = 0; i < ledCount; i++) {
      leds[i] = CRGB::Black;
    }
    // LOG_DEBUG(LABEL, "Updating at offset " << offset);
    ledUpdateMillis = millis();
    for (int i = 0; i < ledCount; i++) {
      leds[i] = scroll((i + offset) % ledCount);
    }
    offset = (offset + 1) % ledCount;
    FastLED.show();
  }
}

int samples = 0;
int reads = 0;
int lastUpdate = millis();
void updateLedsWithAudio(AudioData* audioData) {
  auto now = millis();
  int average = 0;
  for (int i = 0; i < audioData->size; i++) {
    average += abs(audioData->samples[i]);
  }
  samples += audioData->size;
  reads++;
  auto time = (now - lastUpdate) / 1000.0f;
  if (time > 1) {
    auto sampleRate = samples / time;
    auto readRate = reads / time;
    auto averageSamples = samples / reads;
    ESP_LOGI(
        LABEL,
        "Time: %.2f, Samples: %d, Reads: %d, Sample rate: %.2f. Read rate: %.2f. Average sample "
        "size: %d",
        time, samples, reads, sampleRate, readRate, averageSamples);
    samples = 0;
    reads = 0;
    lastUpdate = millis();
  }
  average /= audioData->size;
  // ESP_LOGI(LABEL, "Read %d samples. Average: %d", audioData->size, average);
}
