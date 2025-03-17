
#pragma once

#include "audio.h"

void setupLeds(int ledCount);
void setIntensityThreshold(int threshold);

void ledsAll();
void ledsOneByOne();
void turnOffLeds();
void updateLedsWithAudio(AudioData* audioData);