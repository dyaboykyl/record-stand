#include <Arduino.h>
#include <esp_log.h>
#include <lvgl.h>

#include "screen/calibration.h"
#include "screen/screen.h"
#include "utils.h"

#define LABEL "CalibrationScreen"

// void background() {
//   lv_obj_t *background = lv_obj_create(lv_scr_act());
//   lv_obj_set_scrollbar_mode(background, LV_SCROLLBAR_MODE_OFF);
//   lv_obj_set_size(background, WIDTH, HEIGHT);
//   lv_obj_set_pos(background, 0, 0);
//   lv_obj_set_style_bg_color(background, lv_color_black(), 0);
//   lv_obj_set_style_border_width(background, 0, 0);
//   disable(background);
// }

// void calibrationLine(int radius, lv_color_t color, bool shouldEnable = false) {
//   lv_obj_t *circle = lv_obj_create(lv_screen_active());
//   lv_obj_set_size(circle, radius * 2, radius * 2);
//   lv_obj_set_pos(circle, WIDTH / 2 - radius, HEIGHT / 2 - radius);
//   lv_obj_set_style_bg_color(circle, color, 0);
//   lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
//   lv_obj_set_style_border_width(circle, 0, 0);
//   // if (!enable) disable(circle);
//   enable(circle);
// }

// void calibrationLines() {
//   srand(time(NULL));
//   for (int i = 250; i >= 10; i -= 20) {
//     calibrationLine(i, lv_color_hsv_to_rgb(rand() % 255, 150, 200), i == 30);
//   }
// }
lv_obj_t* parent;
lv_obj_t* leftButton;
static void moveButtonHandler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  ESP_LOGI(LABEL, "moveButtonHandler: %d", code);
  if (code != LV_EVENT_CLICKED) {
    return;
  }

  int x = 0, y = 0, r = 0;
  auto direction = (char*)lv_event_get_user_data(e);
  ESP_LOGI(LABEL, "Move %c pressed", direction[0]);
  switch (direction[0]) {
    case '>':
      x = 1;
      break;
    case '<':
      x = -1;
      break;
    case '^':
      y = -1;
      break;
    case 'v':
      y = 1;
      break;
    case 'r':
      r = 10;
      break;
    case 'l':
      r = -10;
      break;
    default:
      ESP_LOGW(LABEL, "Unknown direction");
  }
  auto xTranslate = lv_obj_get_style_translate_x(parent, 0) + x;
  auto yTranslate = lv_obj_get_style_translate_y(parent, 0) + y;
  ESP_LOGI(LABEL, "Move %c pressed. New x: %d new y: %d", direction[0], xTranslate, yTranslate);
  lv_obj_set_style_transform_pivot_x(parent, 240, 0);
  lv_obj_set_style_transform_pivot_y(parent, 240, 0);
  lv_obj_set_style_transform_rotation(parent, r + lv_obj_get_style_transform_rotation(parent, 0),
                                      0);
  lv_obj_set_style_translate_x(parent, xTranslate, 0);
  lv_obj_set_style_translate_y(parent, yTranslate, 0);
}

int buttonSize = 70;
int buttonOffset = 50;
void moveButton(char* direction, int xOffset, int yOffset) {
  lv_obj_t* button = lv_button_create(parent);
  if (direction[0] == '<') {
    leftButton = button;
  }
  lv_obj_add_event_cb(button, moveButtonHandler, LV_EVENT_CLICKED, direction);
  lv_obj_align(button, LV_ALIGN_CENTER, xOffset, yOffset);
  lv_obj_remove_flag(button, LV_OBJ_FLAG_PRESS_LOCK);
  enable(button);
  lv_obj_set_size(button, 70, 60);

  lv_obj_t* label = lv_label_create(button);
  lv_label_set_text(label, direction);
  // lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
  lv_obj_center(label);
  ESP_LOGI(LABEL, "Created button for %s", direction);
  Serial.println("Created button");
}

void buildCalibrationScreen() {
  // disable(lv_screen_active());
  parent = lv_obj_create(lv_screen_active());
  lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(parent, SCREEN_DIAMETER * 1.15, SCREEN_DIAMETER * 1.15);
  // lv_obj_set_style_opa(parent, LV_OPA_0, 0);
  calibrationLines();
  moveButton(">", buttonOffset + buttonSize / 2, 0);
  moveButton("<", -buttonOffset - buttonSize / 2, 0);
  moveButton("^", 0, -buttonOffset - buttonSize / 2);
  moveButton("v", 0, buttonOffset + buttonSize / 2);
  moveButton("r", buttonOffset * 2, buttonOffset * 2);
  moveButton("l", -buttonOffset * 2, buttonOffset * 2);
  // background();
}
