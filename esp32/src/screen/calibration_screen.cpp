#include "screen/calibration_screen.h"

#include <Arduino.h>
#include <esp_log.h>
#include <lvgl.h>

#include "Observable.h"
#include "logging.h"
#include "screen/ScreenState.h"
#include "screen/calibration.h"
#include "screen/screen.h"
#include "utils.h"

extern ScreenState screenState;

static auto logger = Logger("CalibrationScreen");
static lv_obj_t* parent = nullptr;

void CalibrationState::init() { logger.info("[init]"); }
const lv_color_t CALIBRATION_LINE_COLOR = lv_palette_main(LV_PALETTE_CYAN);
const int BORDER_WIDTH = 4;

void circlularCalibrationLine(int radius, bool shouldEnable = false) {
  lv_obj_t* circle = lv_obj_create(parent);
  lv_obj_set_size(circle, radius * 2, radius * 2);
  lv_obj_align(circle, LV_ALIGN_CENTER, 0, 0);
  // lv_obj_set_pos(circle, SCREEN_RADIUS - radius, SCREEN_RADIUS - radius);
  lv_obj_set_style_border_color(circle, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_bg_opa(circle, LV_OPA_TRANSP, 0);
  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(circle, 3, 0);
  // if (!enable) disable(circle);
}

void horizontalLine() {
  lv_obj_t* horizontal_line = lv_obj_create(parent);
  lv_obj_set_size(horizontal_line, SCREEN_DIAMETER, BORDER_WIDTH);
  lv_obj_align(horizontal_line, LV_ALIGN_CENTER, 0, -BORDER_WIDTH / 2);
  // lv_obj_set_pos(horizontal_line, 0, SCREEN_RADIUS - 1);
  lv_obj_set_style_bg_color(horizontal_line, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_border_color(horizontal_line, CALIBRATION_LINE_COLOR, 0);
}

void verticalLine() {
  lv_obj_t* vertical_line = lv_obj_create(parent);
  lv_obj_set_size(vertical_line, BORDER_WIDTH, SCREEN_DIAMETER);
  lv_obj_align(vertical_line, LV_ALIGN_CENTER, -BORDER_WIDTH / 2, 0);
  // lv_obj_set_pos(vertical_line, SCREEN_RADIUS - 1, 0);
  lv_obj_set_style_bg_color(vertical_line, CALIBRATION_LINE_COLOR, 0);
  lv_obj_set_style_border_color(vertical_line, CALIBRATION_LINE_COLOR, 0);
}

void calibrationLines() {
  circlularCalibrationLine(220);
  verticalLine();
  horizontalLine();
}

static void moveButtonHandler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_CLICKED) {
    return;
  }

  int x = 0, y = 0, r = 0;
  auto direction = (char*)lv_event_get_user_data(e);
  auto offset = screenState.screenOffset.get();
  if (direction == LV_SYMBOL_RIGHT) {
    x = 1;
  } else if (direction == LV_SYMBOL_LEFT) {
    x = -1;
  } else if (direction == LV_SYMBOL_UP) {
    y = -1;
  } else if (direction == LV_SYMBOL_DOWN) {
    y = 1;
  } else if (direction == LV_SYMBOL_REFRESH) {
    y = -offset.y;
    x = -offset.x;
  }
  //   break;
  // case 'r':
  //   r = 10;
  //   break;
  // case 'l':
  //   r = -10;
  //   break;
  // default:
  //   logger.warn("Unknown direction");
  // }
  auto xTranslate = offset.x + x;
  auto yTranslate = offset.y + y;
  logger.debug("Move %c pressed. New x: %d new y: %d", direction[0], xTranslate, yTranslate);
  screenState.screenOffset.set({xTranslate, yTranslate});
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
  lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
  lv_obj_center(label);
  logger.info("Created button for %s", direction);
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

  screenState.screenOffset.subscribe(
      [offsetLabel](const Pair& offset) {
        lv_label_set_text(offsetLabel, ("Offset " + pairToString(offset)).c_str());
      },
      true);
}

lv_obj_t* calibrationScreen = nullptr;
lv_obj_t* buildCalibrationScreen() {
  if (calibrationScreen == nullptr) {
    logger.info("Creating");
    calibrationScreen = lv_obj_create(NULL);
    parent = lv_obj_create(calibrationScreen);
    static auto calibrationScreenState = CalibrationState();
    calibrationScreenState.init();
    lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(parent, SCREEN_DIAMETER * 1.15, SCREEN_DIAMETER * 1.15);
    calibrationLines();
    title();
    offsetText();
    moveButton(LV_SYMBOL_RIGHT, buttonOffset + buttonSize / 2, 0);
    moveButton(LV_SYMBOL_LEFT, -buttonOffset - buttonSize / 2, 0);
    moveButton(LV_SYMBOL_UP, 0, -buttonOffset - buttonSize / 2);
    moveButton(LV_SYMBOL_DOWN, 0, buttonOffset + buttonSize / 2);
    moveButton(LV_SYMBOL_REFRESH, 0, 0);
  }
  logger.info("Loading");
  lv_screen_load_anim(calibrationScreen, LV_SCR_LOAD_ANIM_OVER_TOP, 250, 0, false);
  return parent;
}
