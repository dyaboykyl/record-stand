
#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <esp_log.h>

#include "screen/gfx_screen.h"

#define LABEL "Main"
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

// 2.1" 480x480 round display use CST826 touchscreen with I2C Address at 0x15
#define I2C_TOUCH_ADDR 0x15  // often but not always 0x48!

Arduino_XCA9554SWSPI *expander2 =
    new Arduino_XCA9554SWSPI(PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI, &Wire, 0x3F);

Arduino_ESP32RGBPanel *rgbpanel2 = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK, TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5, TFT_G0, TFT_G1,
    TFT_G2, TFT_G3, TFT_G4, TFT_G5, TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5, 1 /* hync_polarity */,
    46 /* 46hsync_front_porch */, 2 /* hsync_pulse_width */, 44 /* 44hsync_back_porch */,
    1 /* vsync_polarity */, 50 /* 50vsync_front_porch */, 16 /* vsync_pulse_width */,
    16 /* vsync_back_porch */);

Arduino_RGB_Display *gfx2 = new Arduino_RGB_Display(
    // 2.1" 480x480 round display
    480 /* width */, 480 /* height */, rgbpanel2, 0 /* rotation */, true /* auto_flush */,
    expander2, GFX_NOT_DEFINED /* RST */, TL021WVC02_init_operations,
    sizeof(TL021WVC02_init_operations));

Adafruit_CST8XX cst_ctp2 = Adafruit_CST8XX();

int vprintfSerial2(const char *fmt, va_list args) {
  char log_print_buffer[256];
  vsprintf(log_print_buffer, fmt, args);

  Serial.print(log_print_buffer);

  return 0;
}

void initLog() {
  Wire.begin();
  Serial.begin(115200);

  esp_log_set_vprintf(vprintfSerial2);
  esp_log_level_set("*", ESP_LOG_DEBUG);

  Serial.setDebugOutput(true);

  auto start = millis();
  while (!Serial && millis() - start < 500) {
    delay(10);
  }

  Serial.println("Serial: Logging ready");
  ESP_LOGI(LABEL, "Logging ready");
}

void setup() {
  initLog();
  initGfx(gfx2);
  drawGfxScreen();
}

void loop() {
  delay(5);
  // drawGfxScreen();
}