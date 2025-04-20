
#include <lvgl.h>

#include "utils.h"

void background() {
  lv_obj_t *background = lv_obj_create(lv_scr_act());
  lv_obj_set_scrollbar_mode(background, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_size(background, 480, 480);
  lv_obj_set_pos(background, 0, 0);
  lv_obj_set_style_bg_color(background, lv_color_black(), 0);
  lv_obj_set_style_border_width(background, 0, 0);
  disable(background);
}

void calibrationLine(int radius, lv_color_t color, bool shouldEnable = false) {
  lv_obj_t *circle = lv_obj_create(lv_screen_active());
  lv_obj_set_size(circle, radius * 2, radius * 2);
  lv_obj_set_pos(circle, 240 - radius, 240 - radius);
  lv_obj_set_style_bg_color(circle, color, 0);
  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(circle, 0, 0);
  // if (!enable) disable(circle);
  enable(circle);
}

void calibrationLines() {
  srand(time(NULL));
  for (int i = 250; i >= 10; i -= 20) {
    calibrationLine(i, lv_color_hsv_to_rgb(rand() % 255, 150, 200), i == 30);
  }
}

void buildCalibrationScreen() {
  disable(lv_screen_active());
  background();
  calibrationLines();
}
