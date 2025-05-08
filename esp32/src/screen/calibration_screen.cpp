#include "screen/calibration_screen.h"

#include <Arduino.h>
#include <esp_log.h>
#include <lvgl.h>

#include "Observable.h"
#include "logging.h"
#include "screen/ScreenController.h"
#include "screen/calibration.h"
#include "screen/screen.h"
#include "utils.h"

#define LABEL "CalibrationScreen"

extern ScreenController screenController;

static auto logger = Logger(LABEL);
lv_obj_t* parent;

void CalibrationScreenController::init() {
  logger.info("[init]");
  screenController->screenOffset.subscribe(
      [this](const Pair& offset) {
        logger.debug("subscribe offset: %d, %d", get<0>(offset), get<1>(offset));
        // lv_obj_set_style_transform_pivot_x(parent, SCREEN_RADIUS, 0);
        // lv_obj_set_style_transform_pivot_y(parent, SCREEN_RADIUS, 0);
        lv_obj_set_style_translate_x(parent, get<0>(offset), 0);
        lv_obj_set_style_translate_y(parent, get<1>(offset), 0);
      },
      true);
}

static void moveButtonHandler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_CLICKED) {
    return;
  }

  int x = 0, y = 0, r = 0;
  auto direction = (char*)lv_event_get_user_data(e);
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
  auto offset = screenController.screenOffset.get();
  auto xTranslate = get<0>(offset) + x;
  auto yTranslate = get<1>(offset) + y;
  ESP_LOGI(LABEL, "Move %c pressed. New x: %d new y: %d", direction[0], xTranslate, yTranslate);
  // lv_obj_set_style_transform_rotation(parent, r + lv_obj_get_style_transform_rotation(parent, 0),
  //                                     0);
  screenController.screenOffset.set({xTranslate, yTranslate});
  // lv_obj_set_style_transform_pivot_x(parent, SCREEN_RADIUS, 0);
  // lv_obj_set_style_transform_pivot_y(parent, SCREEN_RADIUS, 0);
  // lv_obj_set_style_translate_x(parent, xTranslate, 0);
  // lv_obj_set_style_translate_y(parent, yTranslate, 0);
}

int buttonSize = 70;
int buttonOffset = 50;
void moveButton(const char* direction, int xOffset, int yOffset) {
  lv_obj_t* button = lv_button_create(parent);
  lv_obj_add_event_cb(button, moveButtonHandler, LV_EVENT_CLICKED, (void*)direction);
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

void title() {
  lv_obj_t* title = lv_label_create(parent);
  lv_obj_set_style_text_color(title, lv_color_black(), 0);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_28, 0);
  lv_obj_set_style_align(title, LV_ALIGN_CENTER, 0);
  lv_label_set_text(title, "Alignment");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 80);
}

void offsetText() {
  lv_obj_t* offsetLabel = lv_label_create(parent);
  lv_obj_set_style_text_color(offsetLabel, lv_color_black(), 0);
  lv_obj_set_style_text_font(offsetLabel, &lv_font_montserrat_18, 0);
  lv_obj_set_style_align(offsetLabel, LV_ALIGN_CENTER, 0);
  lv_obj_align(offsetLabel, LV_ALIGN_TOP_LEFT, 350, 150);

  screenController.screenOffset.subscribe(
      [offsetLabel](const Pair& offset) {
        lv_label_set_text(offsetLabel, ("Offset " + pairToString(offset)).c_str());
      },
      true);
}

void buildCalibrationScreen() {
  // disable(lv_screen_active());
  parent = lv_obj_create(lv_screen_active());
  static auto calibrationScreenController = CalibrationScreenController(&screenController);
  calibrationScreenController.init();
  lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(parent, SCREEN_DIAMETER * 1.15, SCREEN_DIAMETER * 1.15);
  // lv_obj_set_style_opa(parent, LV_OPA_0, 0);
  calibrationLines();
  title();
  offsetText();
  moveButton(">", buttonOffset + buttonSize / 2, 0);
  moveButton("<", -buttonOffset - buttonSize / 2, 0);
  moveButton("^", 0, -buttonOffset - buttonSize / 2);
  moveButton("v", 0, buttonOffset + buttonSize / 2);
  // moveButton("r", buttonOffset * 2, buttonOffset * 2);
  // moveButton("l", -buttonOffset * 2, buttonOffset * 2);
  // background();
}
