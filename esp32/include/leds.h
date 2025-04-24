
#pragma once

#include "audio.h"

void setupLeds(int ledCount);
void setIntensityThreshold(int threshold);

void ledsAll(bool update = true);
void ledsOneByOne();
void turnOffLeds();
void updateLedsWithAudio(AudioData* audioData);