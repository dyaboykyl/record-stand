
#include "screen/main_screen.h"

#include <lvgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "calibration.h"
#include "screen/screen.h"
#include "utils.h"

using namespace std;

static auto logger = Logger("MainScreen");

string songTitle = "Some very very long song title";
string artistName = "The very long artists of song";
string action = "Listening";

static lv_anim_t listeningAnimation;
lv_obj_t *mainScreen = nullptr;
static lv_obj_t *parent = nullptr;

void nowPlayingLabel(lv_obj_t *container) {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_18);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *label = lv_label_create(container);
  lv_label_set_text(label, "Now Playing");
  lv_obj_add_style(label, &style, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -100);
}

void songTitleLabel(lv_obj_t *container) {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_32);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *label = lv_label_create(container);
  lv_label_set_text(label, songTitle.c_str());
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(label, 350);
  lv_obj_add_style(label, &style, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -60);
}

void buildArtistLabel(lv_obj_t *container) {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_28);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *label = lv_label_create(container);
  lv_label_set_text(label, artistName.c_str());
  lv_obj_add_style(label, &style, 0);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(label, 350);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
}

void nowPlayingInfo() {
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_layout(container, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_size(container, 350, 300);
  lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align(container, LV_ALIGN_CENTER, 0, -40);

  nowPlayingLabel(container);
  songTitleLabel(container);
  buildArtistLabel(container);
}

void actionLedAnimationCallback(void *actionLed, int32_t value) {
  lv_obj_set_style_opa((lv_obj_t *)actionLed, value, 0);
}

void ledAnimation(lv_obj_t *actionLed) {
  // LV_LOG_USER("Creating LED animation");
  lv_anim_init(&listeningAnimation);
  // lv_anim_set_exec_cb(&animation, scroll_anim_y_cb);
  lv_anim_set_duration(&listeningAnimation, 500);
  lv_anim_set_playback_duration(&listeningAnimation, 500);
  lv_anim_set_repeat_delay(&listeningAnimation, 20);
  lv_anim_set_repeat_count(&listeningAnimation, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_values(&listeningAnimation, 0, 255);
  lv_anim_set_var(&listeningAnimation, actionLed);
  // lv_anim_set_completed_cb(&animation, slideshow_anim_completed_cb);
  lv_anim_set_path_cb(&listeningAnimation, lv_anim_path_ease_in_out);
  lv_anim_set_exec_cb(&listeningAnimation, actionLedAnimationCallback);
  lv_anim_start(&listeningAnimation);
}

void listeningLed() {
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_layout(container, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
  lv_obj_set_size(container, 200, 50);
  lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align(container, LV_ALIGN_CENTER, 0, 150);

  lv_obj_t *actionLed = lv_led_create(container);
  lv_led_set_color(actionLed, lv_color_make(255, 0, 0));
  lv_obj_set_size(actionLed, 10, 10);

  static lv_style_t labelStyle;
  lv_style_init(&labelStyle);
  lv_style_set_text_font(&labelStyle, &lv_font_montserrat_18);
  lv_style_set_text_align(&labelStyle, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *label = lv_label_create(container);
  lv_label_set_text(label, action.c_str());
  lv_obj_add_style(label, &labelStyle, 0);
  // lv_obj_align(label, LV_ALIGN_CENTER, 0, 20);

  ledAnimation(actionLed);
}

void settingsButton() {
  lv_obj_t *button = lv_btn_create(parent);
  lv_obj_set_size(button, 100, 50);
  lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -50);

  static lv_style_t buttonStyle;
  lv_style_init(&buttonStyle);
  lv_style_set_text_font(&buttonStyle, &lv_font_montserrat_18);
  lv_style_set_text_align(&buttonStyle, LV_TEXT_ALIGN_CENTER);
  lv_style_set_radius(&buttonStyle, 10);

  lv_obj_add_style(button, &buttonStyle, 0);

  lv_obj_t *label = lv_label_create(button);
  lv_label_set_text(label, LV_SYMBOL_SETTINGS);
  lv_obj_center(label);

  lv_obj_add_event_cb(
      button,
      [](lv_event_t *e) {
        logger.info("Settings button clicked");
        screenState.goToScreen(Screen::SETTINGS);
      },
      LV_EVENT_CLICKED, NULL);
}

lv_obj_t *buildMainScreen() {
  auto animation = LV_SCR_LOAD_ANIM_OVER_TOP;
  if (mainScreen == nullptr) {
    logger.info("Creating");
    animation = LV_SCR_LOAD_ANIM_NONE;
    mainScreen = lv_obj_create(NULL);
    parent = lv_obj_create(mainScreen);
    lv_obj_align(parent, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(parent, SCREEN_DIAMETER * 1.15, SCREEN_DIAMETER * 1.15);
    // lv_obj_set_style_bg_color(mainScreen, lv_color_white(), 0);
    nowPlayingInfo();
    listeningLed();
    // title();
  }
  logger.info("Loading");
  lv_screen_load_anim(mainScreen, animation, 250, 0, false);
  return parent;
}
