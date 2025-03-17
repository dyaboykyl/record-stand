
#include "leds.h"

// #include <EasyLogger.h>
#include <FastLED.h>

#define LABEL "Leds"

#define BRIGHTNESS 255
#define LED_PIN MISO
#define MAX_LEDS 150

const int ledCount = 16;
const int ledsPerGroup = 4;
const int ledGroupsCount = ledCount / ledsPerGroup;
int ledGroups[ledGroupsCount][ledsPerGroup];

CRGB leds[MAX_LEDS];
unsigned int ledUpdateMillis = 0;

int intensityThreshold = 300;

void turnOffLeds() {
  ESP_LOGI(LABEL, "Turning off leds");
  for (int i = 0; i < MAX_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void createLedGroup(int start, int groupNumber, int direction, int jump) {
  auto group = ledGroups[groupNumber];
  int end = start + (ledsPerGroup * direction * jump);
  int pos = 0;
  for (int i = start; direction == 1 ? i < end : i > end; i += (direction * jump)) {
    group[pos++] = i;
  }
}

void createLedGroups() {
  createLedGroup(0, 0, 1, 1);
  createLedGroup(ledsPerGroup * 2 - 1, 1, -1, 1);
  createLedGroup(ledsPerGroup * 2, 2, 1, 1);
  createLedGroup(ledsPerGroup * 4 - 1, 3, -1, 1);
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
  // ledCount = totalLeds;
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, ledCount).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  createLedGroups();
  turnOffLeds();
  ESP_LOGI(LABEL, "Setup complete");
}

void setIntensityThreshold(int threshold) {
  intensityThreshold = threshold;
  ESP_LOGI(LABEL, "Intensity threshold set to %d", intensityThreshold);
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

int runningAverageIntensity = 1;
void updateRunningAverageIntensity(int average) {
  runningAverageIntensity = (runningAverageIntensity * .9 + average * .1);
  ESP_LOGI(LABEL, "Running average intensity: %d", runningAverageIntensity);
}

void updateLedsWithIntensity(int average) {
  for (int i = 0; i < ledGroupsCount; i++) {
    for (int j = 0; j < ledsPerGroup; j++) {
      int pos = ledGroups[i][j];
      // ESP_LOGD(LABEL, "Updating led %d", pos);
      if (average / (float)intensityThreshold >= j / 4.0f + .1) {
        leds[pos] = scroll(j * 4);
      } else {
        leds[pos] = CRGB::Black;
      }
    }
  }
  FastLED.show();
}

int samples = 0;
int reads = 0;
int intensity = 0;
int lastUpdate = millis();
void updateLedsWithAudio(AudioData* audioData) {
  auto now = millis();
  int average = 0;
  for (int i = 0; i < audioData->size; i++) {
    average += abs(audioData->samples[i]);
  }
  average /= audioData->size;
  updateLedsWithIntensity(average);

  samples += audioData->size;
  reads++;
  auto time = (now - lastUpdate) / 1000.0f;
  intensity += average;
  if (time > 1) {
    auto sampleRate = samples / time;
    auto readRate = reads / time;
    auto averageIntensity = intensity / reads;
    ESP_LOGI(LABEL,
             "Time: %.2f, Samples: %d, Reads: %d, Sample rate: %.2f. Read rate: %.2f. Average "
             "intensity %d",
             time, samples, reads, sampleRate, readRate, averageIntensity);
    samples = 0;
    reads = 0;
    intensity = 0;
    lastUpdate = millis();
  }
  // ESP_LOGI(LABEL, "Read %d samples. Average: %d", audioData->size, average);
  // updateRunningAverageIntensity(average);
}
