
#define LOGGING true

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <EasyLogger.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <acr.h>
#include <driver/adc.h>
#include <wav.h>

// Most touchscreens use FocalTouch with I2C Address often but not always 0x48!
#define I2C_TOUCH_ADDR 0x48

// global variables
Arduino_XCA9554SWSPI* expander =
    new Arduino_XCA9554SWSPI(PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI, &Wire, 0x3F);
Arduino_ESP32RGBPanel* rgbpanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK, TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5, TFT_G0, TFT_G1,
    TFT_G2, TFT_G3, TFT_G4, TFT_G5, TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5, 1 /* hync_polarity */,
    46 /* 46hsync_front_porch */, 2 /* hsync_pulse_width */, 44 /* 44hsync_back_porch */,
    1 /* vsync_polarity */, 50 /* 50vsync_front_porch */, 16 /* vsync_pulse_width */,
    16 /* vsync_back_porch */);
Arduino_RGB_Display* gfx = new Arduino_RGB_Display(
    // 2.1" 480x480 round display
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */, expander,
    GFX_NOT_DEFINED /* RST */, TL021WVC02_init_operations, sizeof(TL021WVC02_init_operations));

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

bool taskRunning = false;

// functions

void drawText(String text) {
  LOG_DEBUG("MAIN", "Drawing text: " << text);
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextSize(2);
  sprite.drawString(text, 240, 240);
  gfx->draw16bitBeRGBBitmap(00, 00, (uint16_t*)sprite.getPointer(), 480, 480);
}

void displaySongInfo(SongInfo info) {
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextSize(1);
  sprite.drawString("Song: " + info.song, 240, 220);
  sprite.drawString("Artist: " + info.artist, 240, 280);
  gfx->draw16bitBeRGBBitmap(00, 00, (uint16_t*)sprite.getPointer(), 480, 480);
}

TaskHandle_t wavTaskHandle;
void wavTask(void* params) {
  LOG_INFO("WAV_TASK", "Running WAV task");
  startNewWav();

  unsigned long start = millis();
  unsigned long currentMs = start;
  int value = 0;
  int offset = pow(2, 15) * .94 * 0;
  int avg = offset;
  float alpha = .6;

  while (!wavFilled()) {  // && currentMs - start < MS_TO_SAMPLE) {
    adc2_get_raw(ADC2_CHANNEL_5, ADC_WIDTH_BIT_12, &value);
    int mappedValue = map(value, 0, 4096, 0, 65536);
    avg = avg * alpha + (1 - alpha) * mappedValue;
    addWavSample(mappedValue - avg - offset);
    // currentMs = millis();
  }
  int totalMs = millis() - start;

  LOG_INFO("WAV", "Finished writing WAV file");
  finishWav(totalMs);
  drawText("Identifying...");
  SongInfo result = identifySongV2(getWavData(), getWavFileSize(), true);
  LOG_INFO("MAIN", "Uploaded song");
  SongInfo result2 = identifySongV2(getWavData(), getWavFileSize());
  LOG_INFO("MAIN", "ACR server: Artist: " + result2.artist + ",Song: " << result2.song);
  displaySongInfo(result);

  taskRunning = false;

  for (;;) delay(1000);
}

void setup() {
  Serial.begin(115200);
  // while (!Serial) delay(10);
  delay(1000);

  LOG_INFO("INIT", "[setup]");
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif
  Wire.setClock(1000000);  // speed up I2C
  if (!gfx->begin()) {
    LOG_ERROR("INIT", "gfx->begin() failed!");
  }
  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc2_config_channel_atten(ADC2_CHANNEL_5, ADC_ATTEN_DB_11);

  gfx->fillScreen(TFT_DARKCYAN);
  sprite.createSprite(480, 480);
  sprite.setTextSize(1);
  sprite.setTextFont(4);
  sprite.setTextDatum(MC_DATUM);
  sprite.setTextColor(TFT_WHITE);

  LOG_INFO("INIT", "PSRAM found:" << psramFound());
  if (psramFound()) {
    LOG_INFO("INIT", "Total PSRAM:" << ESP.getPsramSize());
    LOG_INFO("INIT", "Free PSRAM:" << ESP.getFreePsram());
    allocateWavSpace();
    LOG_INFO("INIT", "Free PSRAM:" << ESP.getFreePsram());
  }

  analogReadResolution(16);
  // connectToWifi();
  drawText("Ready");
}

void loop() {
  // //LOG_INFO("LOOP", "looping...");
  if (!taskRunning) {
    if (!expander->digitalRead(PCA_BUTTON_DOWN)) {
      LOG_INFO("LOOP", "BUTTON_ONE_PRESSED");
      taskRunning = true;
      drawText("Listening...");
      xTaskCreatePinnedToCore(wavTask, "WavTask", 10000, NULL, 1, &wavTaskHandle, 1);
      // wavTask();
      // taskRunning = false;
      delay(100);
    }
  }

  if (!taskRunning) {
    if (!expander->digitalRead(PCA_BUTTON_UP)) {
      taskRunning = true;
      drawText("Identifying...");
      SongInfo result = identifySongV2(getWavData(), getWavFileSize());
      LOG_INFO("MAIN", "Artist: " + result.artist + ", Song: " << result.song);
      displaySongInfo(result);
      // writeWavFileToSerial();
      taskRunning = false;
      delay(100);
    }
  }
  delay(100);
}
