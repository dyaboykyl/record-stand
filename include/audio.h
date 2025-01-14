#pragma once

#include <stdint.h>

typedef struct AudioData {
  int16_t* samples;
  int size;
} AudioData;

void initAudio();

AudioData* readI2sAudio();

int16_t readAudio(bool shouldScale = true, bool notchFilter = true);
void calculateNoiseOffset();
int getNoiseOffset();