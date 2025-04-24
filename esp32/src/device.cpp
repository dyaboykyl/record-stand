
#include "device.h"

#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <esp_log.h>

#include "Button.h"

#define LABEL "Device"

Button button1("DOWN", PCA_BUTTON_DOWN);
Button button2("UP", PCA_BUTTON_UP);

Arduino_XCA9554SWSPI *expander =
    new Arduino_XCA9554SWSPI(PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI, &Wire, 0x3D);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK, TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5, TFT_G0, TFT_G1,
    TFT_G2, TFT_G3, TFT_G4, TFT_G5, TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5, 1 /* hync_polarity */,
    46 /* 46hsync_front_porch */, 2 /* hsync_pulse_width */, 44 /* 44hsync_back_porch */,
    1 /* vsync_polarity */, 50 /* 50vsync_front_porch */, 16 /* vsync_pulse_width */,
    16 /* vsync_back_porch */);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    // 2.1" 480x480 round display
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */, expander,
    GFX_NOT_DEFINED /* RST */, TL021WVC02_init_operations, sizeof(TL021WVC02_init_operations));

Adafruit_CST8XX cst_ctp = Adafruit_CST8XX();

bool initTouch() {
  if (!cst_ctp.begin(&Wire, I2C_TOUCH_ADDR)) {
    // LOG_WARNING(LABEL, "No Touchscreen found at address 0x");
    Serial.println(I2C_TOUCH_ADDR, HEX);
    return false;
  } else {
    ESP_LOGI(LABEL, "CST826 Touchscreen found");
    return true;
  }
}

void initButtons() {
  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);
  ESP_LOGI(LABEL, "Buttons initialized");
}