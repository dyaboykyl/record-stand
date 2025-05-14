#include "screen/settings_screen.h"

#include <lvgl.h>

#include "logging.h"
#include "screen/screen.h"

using namespace std;

static auto logger = Logger("SettingsScreen");

lv_obj_t *settingsScreen = nullptr;
static lv_obj_t *parent = nullptr;

void calibrateButton() {
  static lv_obj_t *button = lv_button_create(parent);
  lv_obj_align(button, LV_ALIGN_CENTER, 0, -100);
  lv_obj_remove_flag(button, LV_OBJ_FLAG_PRESS_LOCK);
  enable(button);
  lv_obj_set_size(button, 120, 60);

  lv_obj_t *label = lv_label_create(button);
  lv_label_set_text(label, "Calibrate");
  lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
  lv_obj_center(label);

  lv_obj_add_event_cb(
      button, [](lv_event_t *e) { appState.goToScreen(Screen::CALIBRATION); }, LV_EVENT_CLICKED,
      NULL);
}

void doneButton() {
  lv_obj_t *button = lv_button_create(parent);
  lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -50);
  lv_obj_remove_flag(button, LV_OBJ_FLAG_PRESS_LOCK);
  enable(button);
  lv_obj_set_size(button, 70, 60);

  lv_obj_t *label = lv_label_create(button);
  lv_label_set_text(label, LV_SYMBOL_HOME);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
  lv_obj_center(label);

  lv_obj_add_event_cb(
      button, [](lv_event_t *e) { appState.goToScreen(Screen::MAIN); }, LV_EVENT_CLICKED, NULL);
}

void title() {
  lv_obj_t *title = lv_label_create(parent);
  lv_obj_set_style_text_color(title, lv_color_black(), 0);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_28, 0);
  lv_obj_set_style_align(title, LV_ALIGN_CENTER, 0);
  lv_label_set_text(title, "Settings");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 80);
}

lv_obj_t *loadSettingsScreen(lv_screen_load_anim_t animation) {
  if (settingsScreen == nullptr) {
    logger.info("Building");
    settingsScreen = lv_obj_create(NULL);
    parent = lv_obj_create(settingsScreen);
    lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(parent, SCREEN_DIAMETER * 1.15, SCREEN_DIAMETER * 1.15);
    // lv_obj_set_style_bg_color(mainScreen, lv_color_white(), 0);

    title();
    calibrateButton();
    doneButton();
  }

  logger.info("Loading animation: %d", animation);
  lv_screen_load_anim(settingsScreen, animation, 250, 0, false);
  return parent;
}