
#include <lvgl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "utils.h"

using namespace std;

string songTitle = "Some very very long song title";
string artistName = "The very long artists of song";
string action = "Listening";

static lv_anim_t listeningAnimation;

void buildNowPlayingLabel(lv_obj_t *container) {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_18);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

  lv_obj_t *label = lv_label_create(container);
  lv_label_set_text(label, "Now Playing");
  lv_obj_add_style(label, &style, 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -100);
}

void buildSongTitleLabel(lv_obj_t *container) {
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

void buildNowPlayingInfo() {
  lv_obj_t *container = lv_obj_create(lv_screen_active());
  lv_obj_set_layout(container, LV_LAYOUT_FLEX);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_size(container, 350, 300);
  lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
  lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align(container, LV_ALIGN_CENTER, 0, -40);

  buildNowPlayingLabel(container);
  buildSongTitleLabel(container);
  buildArtistLabel(container);
}

void actionLedAnimationCallback(void *actionLed, int32_t value) {
  lv_obj_set_style_opa((lv_obj_t *)actionLed, value, 0);
}

void actionAnimation(lv_obj_t *actionLed) {
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

void buildAction() {
  lv_obj_t *container = lv_obj_create(lv_screen_active());
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

  actionAnimation(actionLed);
}

void buildMainScreen() {
  buildNowPlayingInfo();
  buildAction();
  //
}
