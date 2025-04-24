#pragma once

#include <stdint.h>

typedef struct AudioData {
  int16_t* samples;
  int size;
} AudioData;

void initAudio();
void initI2s();
void stopI2s();

AudioData* readI2sAudio();
AudioData* readAnalogAudio();
void plotAudio();

int16_t readAudio(bool shouldScale = true, bool notchFilter = true);
void calculateNoiseOffset();
int getNoiseOffset();