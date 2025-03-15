
#pragma once

#include "audio.h"

void setupLeds(int ledCount);

void ledsAll();
void ledsOneByOne();
void turnOffLeds();
void updateLedsWithAudio(AudioData* audioData);