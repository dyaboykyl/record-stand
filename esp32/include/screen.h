
#pragma once

#include <Arduino_GFX_Library.h>

extern Arduino_XCA9554SWSPI *expander;

void backlightOff();
void backlightOn();
void toggleBacklight();
bool buttonOnePressed();
bool buttonTwoPressed();
void initScreen();
void initButtons();

void reactToTouch();
void rotateScreen();
