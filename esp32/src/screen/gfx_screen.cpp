
#include <Arduino_GFX_Library.h>

#include "screen.h"

static Arduino_RGB_Display *gfx;

void initGfx(Arduino_RGB_Display *gfxToUse) {
  gfx = gfxToUse;
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }
}

void drawGfxScreen() {
  gfx->fillScreen(BLACK);

  // gfx->fillCircle(SCREEN_RADIUS, SCREEN_RADIUS, SCREEN_RADIUS - 10, RED);
  // gfx->fillCircle(SCREEN_RADIUS, SCREEN_RADIUS, SCREEN_RADIUS - 14, BLACK);

  gfx->fillRect(SCREEN_RADIUS - 2, 0, 4, SCREEN_DIAMETER, RED);
  gfx->fillRect(0, SCREEN_RADIUS - 2, SCREEN_DIAMETER, 4, RED);
}