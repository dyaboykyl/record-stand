#pragma once

#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>

#include "Button.h"

// 2.1" 480x480 round display use CST826 touchscreen with I2C Address at 0x15
#define I2C_TOUCH_ADDR 0x15  // often but not always 0x48!

#define SCREEN_DIAMETER 480
#define SCREEN_RADIUS 240

extern Arduino_XCA9554SWSPI *expander;
extern Arduino_ESP32RGBPanel *rgbpanel;
extern Arduino_RGB_Display *gfx;
extern Adafruit_CST8XX cst_ctp;

extern Button button1;
extern Button button2;

void initButtons();
bool initTouch();