
#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <Button.h>
// #include <EasyLogger.h>
#include <TFT_eSPI.h>
#include <demos/lv_demos.h>
#include <examples/lv_examples.h>
#include <lvgl.h>
#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <lvgl/

#include "calibration_screen.h"
#include "utils.h"

#define LABEL "Screen"

// 2.1" 480x480 round display use CST826 touchscreen with I2C Address at 0x15
#define I2C_TOUCH_ADDR 0x15  // often but not always 0x48!

#define DIS_WIDTH_PX 480  /* width of the display, pixels */
#define DIS_HEIGHT_PX 480 /* height of the display, pixels */

struct Pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

Arduino_XCA9554SWSPI *expander =
    new Arduino_XCA9554SWSPI(PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI, &Wire, 0x3F);

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

uint8_t backlightState = HIGH;
uint16_t *colorWheel;
Pixel *colorWheelPixels;
bool touchOK;

Button button1("DOWN", PCA_BUTTON_DOWN);
Button button2("UP", PCA_BUTTON_UP);

#define DRAW_BUF_SIZE (DIS_WIDTH_PX * DIS_HEIGHT_PX / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 8];
uint32_t draw_buf2[DRAW_BUF_SIZE / 8];

static lv_obj_t *keyboard;

// void lv_log_register_print_cb(lv_log_print_g_cb_t print_cb) {
//   // Do nothing, not needed here!
// }

void lvglLog(lv_log_level_t level, const char *buf) {
  if (level >= LV_LOG_LEVEL) {
    Serial.printf(buf);
    Serial.flush();
  }
}

void setupTouch() {
  if (!cst_ctp.begin(&Wire, I2C_TOUCH_ADDR)) {
    // LOG_WARNING(LABEL, "No Touchscreen found at address 0x");
    Serial.println(I2C_TOUCH_ADDR, HEX);
    touchOK = false;
  } else {
    ESP_LOGI(LABEL, "CST826 Touchscreen found");
    touchOK = true;
  }
}

void readTouch(lv_indev_t *indev, lv_indev_data_t *data) {
  if (touchOK && cst_ctp.touched()) {
    CST_TS_Point p = cst_ctp.getPoint(0);
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = p.x;
    data->point.y = p.y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void flushDisplay(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP == 0) /* LV_COLOR_16_SWAP is defined in lv_conf.h */
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#else
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_display_flush_ready(disp);
}

static uint32_t my_tick(void) { return millis(); }

// extern void lv_log_register_print_cb(lv_log_print_g_cb_t print_cb);
void lvglSetup() {
  setupTouch();

  // *********** Start all the setup and initilization needed for lvgl ***********
  lv_init();

#if LV_USE_LOG != 0 /* LV_USE_LOG is the debugging print enable, defined in lv_conf.h */
  lv_log_register_print_cb(lvglLog);  // register the print function for debugging
#endif

  lv_tick_set_cb(my_tick);

  lv_display_t *disp;
  disp = lv_display_create(DIS_WIDTH_PX, DIS_HEIGHT_PX);
  lv_display_set_flush_cb(disp, flushDisplay);

  // draw_buf = (uint32_t *)ps_malloc(DRAW_BUF_SIZE);
  ESP_LOGI(LABEL, "draw_buf: %p size: %d", (unsigned long long)(void const *)draw_buf,
           DRAW_BUF_SIZE);
  lv_display_set_buffers(disp, draw_buf, draw_buf2, sizeof(draw_buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  /*Initialize the (dummy) input device driver*/
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, readTouch);

  /* Option 1: Create a simple label
   * ---------------------
   */
  // lv_obj_t *label = lv_label_create(lv_screen_active());
  // lv_label_set_text(label, "Hello Arduino, I'm LVGL!");
  // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  /* Option 2: Try an example. See all the examples
   *  - Online: https://docs.lvgl.io/master/examples.html
   *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
   * ----------------------------------------------------------------
   */
  // lv_example_get_started_1();
}

void makeKeyboard() {
  keyboard = lv_keyboard_create(lv_scr_act());
  lv_obj_set_style_pad_hor(keyboard, 50, 0);
  lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

// void background() {
//   lv_obj_t *background = lv_obj_create(lv_scr_act());
//   lv_obj_set_scrollbar_mode(background, LV_SCROLLBAR_MODE_OFF);
//   lv_obj_set_size(background, 480, 480);
//   lv_obj_set_pos(background, 0, 0);
//   lv_obj_set_style_bg_color(background, BASE_ONE, 0);
//   // lv_obj_set_style_bg_grad_color(background, BASE_ONE_GRAD, 0);
//   // lv_obj_set_style_bg_grad_dir(background, LV_GRAD_DIR_VER, 0);
//   // lvobdithe

//   // static lv_grad_dsc_t gradient = {
//   //     .dir = LV_GRAD_DIR_VER,
//   //     .stops = {{.color = BASE_ONE, .frac = 0}, {.color = BASE_ONE_GRAD, .frac = 1}},
//   // };
//   // lv_grad_color_t
//   // lv_gradi(&gradient, {BASE_ONE, BASE_ONE_GRAD}, NULL, NULL, 2);

//   // lv_obj_set_style_bg_grad(background, &gradient, 0);
//   lv_obj_set_style_border_width(background, 0, 0);
//   lv_obj_set_style_border_color(background, BASE_ONE, 0);
//   lv_obj_set_style_radius(background, LV_RADIUS_CIRCLE, 0);
//   disable(background);
// }

void addTitle() {
  static lv_style_t titleStyle;
  lv_style_init(&titleStyle);
  lv_style_set_text_font(&titleStyle, &lv_font_montserrat_32);
  lv_style_set_text_align(&titleStyle, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *titleLabel = lv_label_create(lv_screen_active());
  lv_label_set_text(titleLabel, "WIFI");
  // lv_obj_set_style_text_align(titleLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_add_style(titleLabel, &titleStyle, 0);

  lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, -200);
}

uint8_t getRed(uint16_t color) { return ((color & 0xf800) >> 11) << 3; }

uint8_t getGreen(uint16_t color) { return ((color & 0x07e0) >> 5) << 2; }

uint8_t getBlue(uint16_t color) { return (color & 0x001f) << 3; }

// float sierra[] = {0, 0} X 5 3 2 4 5 4 2 2 3 2
float sierraLite[] = {0, 0, .5f, .25f, .25f, 0};
int rows = 2;
int dCol = 3;

int calculateError(uint8_t original) {
  uint8_t rounded = round(original / 255.0f);
  return original - rounded;
}
uint8_t ditherColor(uint8_t (*colorExtractor)(uint16_t), uint16_t rgb565, uint8_t originalColor,
                    float frac) {
  uint8_t color = colorExtractor(rgb565);
  int error = originalColor - color;
  // LOG_DEBUG(LABEL, "error: " << (error));
  return (uint8_t)round(originalColor + (error * frac));
}

void propagateError(uint16_t color, int x, int y, int dx, int dy, float frac, Pixel *pixels,
                    int width, int height) {
  int y2 = y + dy;
  int x2 = y2 * width + x + dx;
  if (x2 > width - 1 || y2 > height - 1) {
    return;
  }

  Pixel *pixel = &pixels[y2 * width + x2];
  // LOG_DEBUG(LABEL, "Before: " << (pixel->red));
  pixel->red = ditherColor(getRed, color, pixel->red, frac);
  pixel->green = ditherColor(getGreen, color, pixel->green, frac);
  pixel->blue = ditherColor(getBlue, color, pixel->blue, frac);
  // LOG_DEBUG(LABEL, "After: " << (pixel->red));
}

void dither(Pixel *pixels, uint16_t *buffer, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int index = y * width + x;
      Pixel pixel = pixels[index];
      uint16_t color = RGB565(pixel.red, pixel.green, pixel.blue);
      // LOG_DEBUG(LABEL, (String)pixel.red + "->" + getRed(color) + "(" + getRed(buffer[index])
      //  << ")");
      buffer[index] = color;

      propagateError(color, x, y, 1, 0, 7 / 16.0f, pixels, width, height);
      propagateError(color, x, y, -1, 1, 3 / 16.f, pixels, width, height);
      propagateError(color, x, y, 0, 1, 5 / 16.f, pixels, width, height);
      propagateError(color, x, y, 1, 1, 1 / 16.f, pixels, width, height);
    }
  }
}

void initButtons() {
  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, backlightState);
  ESP_LOGI(LABEL, "Buttons initialized");
}

extern void buildWifiScreen();

void initScreen() {
  ESP_LOGI(LABEL, "Initializing");

  Wire.setClock(1000000);  // speed up I2C
  if (!gfx->begin()) {
    // LOG_ERROR(LABEL, "gfx->begin() failed!");
  }
  lvglSetup();
  ESP_LOGI(LABEL, "Initialization finished");

  // // wifi setup:
  // //
  // https://github.com/0015/ThatProject/blob/master/ESP32_LVGL/LVGL8/3_BaseProject_Network_Selector/3_BaseProject_Network_Selector.ino#L495
  // background();

  // addTitle();
  buildCalibrationScreen();

  // makeKeyboard();

  // lv_demo_music();
}

CST_TS_Point *prevPoint = NULL;

void reactToTouch() {
  if (touchOK && cst_ctp.touched()) {
    // CST_TS_Point p = cst_ctp.getPoint(0);
    // if (prevPoint != NULL) {
    //   gfx->drawLine(p.x, p.y, prevPoint->x, prevPoint->y, RED);
    // }
    // prevPoint = new CST_TS_Point(p);
    rotateScreen();
  }
}

void writeBacklight(uint8_t state) {
  backlightState = state;
  ESP_LOGI(LABEL, "Writing backlight: ", state);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, backlightState);
}

void updateScreen() { rotateScreen(); }

auto rotation = 5;

extern void rotatePassword();
void rotateScreen() {
  ESP_LOGI(LABEL, "Rotating screen");
  auto screen = lv_screen_active();
  lv_obj_set_style_transform_pivot_x(screen, 240, 0);
  lv_obj_set_style_transform_pivot_y(screen, 240, 0);
  lv_obj_set_style_transform_rotation(screen, rotation * 10, 0);
  rotation += 5;
  ESP_LOGI(LABEL, "Rotated screen");
}

void backlightOff() { writeBacklight(HIGH - LOW); }
void backlightOn() { writeBacklight(HIGH); }
bool buttonOnePressed() { return button1.pressed(); }
bool buttonTwoPressed() { return button2.pressed(); }
void toggleBacklight() { writeBacklight(HIGH - backlightState); }