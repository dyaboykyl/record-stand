
#include "screen/calibration.h"

#include <lvgl.h>

#include "screen.h"

const lv_color_t CALIBRATION_LINE_COLOR = lv_palette_main(LV_PALETTE_CYAN);
const int BORDER_WIDTH = 3;

void circlularCalibrationLine(int radius, bool shouldEnable = false) {
  lv_obj_t *circle = lv_obj_create(lv_screen_active());
  lv_obj_set_size(circle, radius * 2, radius * 2);
  lv_obj_set_pos(circle, SCREEN_RADIUS - radius, SCREEN_RADIUS - radius);
  lv_obj_set_style_border_color(circle, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_bg_opa(circle, LV_OPA_TRANSP, 0);
  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(circle, 3, 0);
  // if (!enable) disable(circle);
}

void horizontalLine() {
  lv_obj_t *horizontal_line = lv_obj_create(lv_screen_active());
  lv_obj_set_size(horizontal_line, SCREEN_DIAMETER, BORDER_WIDTH);
  lv_obj_set_pos(horizontal_line, 0, SCREEN_RADIUS - 1);
  lv_obj_set_style_bg_color(horizontal_line, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_border_color(horizontal_line, CALIBRATION_LINE_COLOR, 0);
}

void verticalLine() {
  lv_obj_t *vertical_line = lv_obj_create(lv_screen_active());
  lv_obj_set_size(vertical_line, BORDER_WIDTH, SCREEN_DIAMETER);
  lv_obj_set_pos(vertical_line, SCREEN_RADIUS - 1, 0);
  lv_obj_set_style_bg_color(vertical_line, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_border_color(vertical_line, CALIBRATION_LINE_COLOR, 0);
}

void calibrationLines() {
  circlularCalibrationLine(220);
  verticalLine();
  horizontalLine();
}