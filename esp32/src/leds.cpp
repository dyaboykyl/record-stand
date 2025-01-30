
#include "leds.h"

#include <EasyLogger.h>
#include <FastLED.h>

#define LABEL "Leds"

#define BRIGHTNESS 255
#define LED_PIN MOSI

int ledCount = 150;
CRGB leds[150];
unsigned int ledUpdateMillis = 0;

void turnOffLeds() {
  for (int i = 0; i < ledCount; i++) {
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
  LOG_INFO(LABEL, "Setup complete");
}

void onLoopLeds() {
  static int offset = 0;
  if ((millis() - ledUpdateMillis) > 250) {
    for (int i = 0; i < ledCount; i++) {
      leds[i] = CRGB::Black;
    }
    LOG_DEBUG(LABEL, "Updating at offset " << offset);
    ledUpdateMillis = millis();
    for (int i = 0; i < ledCount; i++) {
      leds[i] = scroll((i + offset) % ledCount);
    }
    offset = (offset + 1) % ledCount;
    FastLED.show();
  }
  // if (offset == 0) offset = 4;
}
