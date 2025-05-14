

#include "screen/screen.h"

#include <Adafruit_CST8XX.h>
#include <Arduino_GFX_Library.h>
#include <esp_log.h>
#include <lvgl.h>

#include "device.h"
#include "screen/calibration_screen.h"
#include "screen/main_screen.h"

static auto logger = Logger("Screen");

bool touchOK;

uint32_t screenWidth;
uint32_t screenHeight;
const int bufSize = SCREEN_DIAMETER * 40;
lv_display_t *disp;
// lv_color_t *disp_draw_buf;
lv_color_t disp_draw_buf[bufSize];
lv_color_t disp_draw_buf2[bufSize];

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

uint32_t millis_cb(void) { return millis(); }

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
#ifndef DIRECT_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif  // #ifndef DIRECT_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

bool setupGfx() {
  Wire.setClock(1000000);
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif
  if (!gfx->begin()) {
    logger.warn("gfx->begin() failed!");
    return false;
  }
  gfx->fillScreen(WHITE);
  return true;
}

void readTouch(lv_indev_t *indev, lv_indev_data_t *data) {
  if (cst_ctp.touched()) {
    CST_TS_Point p = cst_ctp.getPoint(0);
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = p.x;
    data->point.y = p.y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void setupLvgl() {
  void *lvglSpace = heap_caps_malloc(LV_MEM_SIZE, MALLOC_CAP_SPIRAM);
  logger.info("LV_MEM_ADR: %p", lvglSpace);
  if ((int)lvglSpace != LV_MEM_ADR) {
    logger.warn("LVGL memory allocation not correct");
    delay(20000);
    return;
  }
  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  if (!disp_draw_buf) {
    logger.warn("LVGL disp_draw_buf allocate failed!");
  } else {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, (void *)disp_draw_buf, (void *)disp_draw_buf2, bufSize * 2,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*Initialize the (dummy) input device driver*/
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, readTouch);
  }
}

void initScreen() {
  if (!setupGfx()) {
    logger.warn("GFX setup failed!");
    return;
  }

  initTouch();
  setupLvgl();

  logger.info("Setup done");
}

void startScreen() { appState.goToScreen(Screen::MAIN); }

void screenLoop() { lv_task_handler(); }
