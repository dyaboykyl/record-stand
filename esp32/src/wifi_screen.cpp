
#include <EasyLogger.h>
#include <WiFi.h>
#include <lvgl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstring>
#include <thread>

#include "controller.h"
#include "network.h"
#include "storage.h"
#include "utils.h"

using namespace std;

#define LABEL "WIFI_SCREEN"

lv_obj_t *keyboard;
lv_obj_t *ssidDropdown;
lv_obj_t *passwordTextArea;
lv_obj_t *statusLabel;
lv_obj_t *connectButton;
lv_obj_t *loadingSpinner;

char selectedSsid[100] = "";
bool connecting = false;
int connectionCheckAttempts = 0;

void updateConnectButton() {
  auto password = lv_textarea_get_text(passwordTextArea);
  if (password == NULL || strcmp(password, "") == 0 || strcmp(selectedSsid, "") == 0) {
    disable(connectButton);
    lv_obj_set_style_bg_color(connectButton, DISABLED_COLOR, 0);
    // LV_LOG_USER("Connect button not ready");
  } else {
    enable(connectButton);
    lv_obj_set_style_bg_color(connectButton, lv_color_make(0, 0, 255), 0);
    // LV_LOG_USER("Connect button ready!");
  }
}

void keyboardEventHandler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_CLICKED) {
    return;
  }

  // LV_LOG_USER("Keyboard event: %i", code);
  auto btnId = lv_keyboard_get_selected_button(keyboard);
  if (btnId == LV_BUTTONMATRIX_BUTTON_NONE) {
    return;
  }

  const char *buttonText = lv_keyboard_get_button_text(keyboard, btnId);
  if (strcmp(buttonText, LV_SYMBOL_NEW_LINE) == 0) {
    hide(keyboard);
    lv_keyboard_set_textarea(keyboard, NULL);
  } else {
    updateConnectButton();
  }
}

static void makeKeyboard() {
  keyboard = lv_keyboard_create(lv_scr_act());
  lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_USER_1);
  lv_obj_add_event_cb(keyboard, keyboardEventHandler, LV_EVENT_ALL, NULL);
  lv_obj_set_style_pad_hor(keyboard, 50, 0);
  hide(keyboard);
}

void buildTitle() {
  static lv_style_t titleStyle;
  lv_style_init(&titleStyle);
  lv_style_set_text_font(&titleStyle, &lv_font_montserrat_32);
  lv_style_set_text_align(&titleStyle, LV_TEXT_ALIGN_CENTER);
  lv_style_set_text_color(&titleStyle, BASE_TWO);

  lv_obj_t *titleLabel = lv_label_create(lv_screen_active());
  lv_label_set_text(titleLabel, "WIFI");
  lv_obj_add_style(titleLabel, &titleStyle, 0);
  lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, -200);
}

void buildSsidInputLabel() {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_28);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&style, BASE_TWO);

  static lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "SSID:");
  lv_obj_add_style(label, &style, 0);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 78, 102);
}

void onSsidScanComplete() {
  enable(ssidDropdown);
  lv_obj_set_style_bg_color(ssidDropdown, lv_color_white(), 0);
  lv_dropdown_set_text(ssidDropdown, "Select an SSID");
  lv_dropdown_set_options(ssidDropdown, "");
}

static void ssidDropdownEventHandler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_dropdown_get_selected_str(obj, selectedSsid, sizeof(selectedSsid));
    lv_dropdown_set_text(ssidDropdown, NULL);
    // LV_LOG_USER("Option: %s", selectedSsid);
    updateConnectButton();
    enable(passwordTextArea);
    lv_obj_set_style_bg_color(passwordTextArea, lv_color_white(), 0);
  }
}

void buildSsidInputDropdown() {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_18);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);
  lv_style_set_max_width(&style, 230);
  // lv_sty

  ssidDropdown = lv_dropdown_create(lv_screen_active());
  const char *scanningLabel = "Waiting to scan...";

  lv_dropdown_set_text(ssidDropdown, scanningLabel);
  lv_dropdown_set_options(ssidDropdown, "");
  disable(ssidDropdown);

  lv_obj_set_width(ssidDropdown, 230);
  lv_obj_set_height(ssidDropdown, 43);
  lv_obj_align(ssidDropdown, LV_ALIGN_TOP_LEFT, 170, 96);
  lv_obj_add_event_cb(ssidDropdown, ssidDropdownEventHandler, LV_EVENT_ALL, NULL);
  lv_obj_set_style_bg_color(ssidDropdown, DISABLED_COLOR, 0);

  lv_obj_t *list = lv_dropdown_get_list(ssidDropdown);
  lv_obj_add_style(list, &style, 0);
}

void buildSsidInput() {
  buildSsidInputLabel();
  buildSsidInputDropdown();
}

void buildPasswordInputLabel() {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_28);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);
  lv_style_set_text_color(&style, BASE_TWO);

  static lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Password:");
  lv_obj_add_style(label, &style, 0);
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, 20, 150);
}

void passwordCallback(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = (lv_obj_t *)lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    /*Focus on the clicked text area*/
    if (keyboard != NULL) {
      lv_keyboard_set_textarea(keyboard, ta);
      show(keyboard);
    }
  } else if (code == LV_EVENT_READY) {
    // LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
  }
}

void buildPasswordInputText() {
  passwordTextArea = lv_textarea_create(lv_screen_active());
  lv_textarea_set_text(passwordTextArea, "");
  lv_textarea_set_password_mode(passwordTextArea, true);
  lv_textarea_set_one_line(passwordTextArea, true);
  lv_obj_set_width(passwordTextArea, 230);
  lv_obj_set_pos(passwordTextArea, 170, 150);
  lv_obj_add_event_cb(passwordTextArea, passwordCallback, LV_EVENT_ALL, NULL);
  lv_obj_set_style_bg_color(passwordTextArea, DISABLED_COLOR, 0);
  disable(passwordTextArea);
  // lv_textarea_set_placeholder_text(passwordTextArea, "Password");
}

void buildPasswordInput() {
  buildPasswordInputLabel();
  buildPasswordInputText();
}

void checkConnect(lv_timer_t *timer) {
  // //LOG_DEBUG(LABEL, "Checking scan");
  auto status = getWifiStatus();
  // //LOG_DEBUG(LABEL, "checkConnect: " << status);
  if (status == WL_IDLE_STATUS) {
    return;
  }

  if (status != WL_CONNECTED && connectionCheckAttempts++ < 3) {
    return;
  }

  LOG_DEBUG(LABEL, "Connecting finished: " << status);
  lv_label_set_text(statusLabel, status == WL_CONNECTED ? "Connected" : "Failed to connect");
  lv_timer_delete(timer);
  hide(loadingSpinner);

  if (status == WL_CONNECTED) {
    auto password = lv_textarea_get_text(passwordTextArea);
    if (strlen(password) > 0 && strlen(selectedSsid) > 0) {
      persistWifiCredentials(selectedSsid, password);
    }
  }
}

void tryConnectToWifi(String ssid = "", String password = "") {
  hide(keyboard);
  show(loadingSpinner);
  lv_label_set_text(statusLabel, "Connecting...");

  auto ssidToUse = ssid == "" ? selectedSsid : ssid.c_str();
  auto passwordToUse = password == "" ? lv_textarea_get_text(passwordTextArea) : password.c_str();
  LOG_DEBUG(LABEL, "Connecting to " << selectedSsid << " Password: " << password);
  connectToWifi(ssidToUse, passwordToUse);
  connectionCheckAttempts = 0;
  lv_timer_create(checkConnect, 20, NULL);
}

void connectButtonCallback(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    // LV_LOG_USER("Clicked");
    tryConnectToWifi();
  }
}

void buildConnectButton() {
  connectButton = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(connectButton, connectButtonCallback, LV_EVENT_ALL, NULL);
  lv_obj_align(connectButton, LV_ALIGN_TOP_MID, 0, 200);
  lv_obj_remove_flag(connectButton, LV_OBJ_FLAG_PRESS_LOCK);
  disable(connectButton);
  lv_obj_set_style_bg_color(connectButton, DISABLED_COLOR, 0);

  lv_obj_t *label = lv_label_create(connectButton);
  lv_label_set_text(label, "Connect");
  lv_obj_center(label);
}

void buildConnectionStatus() {
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_font(&style, &lv_font_montserrat_22);
  lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

  statusLabel = lv_label_create(lv_screen_active());
  String statusText = "Not connected";
  lv_label_set_text(statusLabel, statusText.c_str());
  lv_obj_add_style(statusLabel, &style, 0);
  lv_obj_align(statusLabel, LV_ALIGN_TOP_MID, 0, 270);
}

void buildSpinner() {
  loadingSpinner = lv_obj_create(lv_scr_act());
  lv_obj_set_scrollbar_mode(loadingSpinner, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_size(loadingSpinner, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_pos(loadingSpinner, 0, 0);
  lv_obj_set_style_bg_color(loadingSpinner, DISABLED_COLOR, 0);
  lv_obj_set_style_bg_opa(loadingSpinner, LV_OPA_60, 0);
  lv_obj_set_style_radius(loadingSpinner, LV_RADIUS_CIRCLE, 0);

  auto spinner = lv_spinner_create(loadingSpinner);
  lv_obj_set_align(spinner, LV_ALIGN_CENTER);
  lv_obj_set_size(spinner, 75, 75);
  lv_obj_set_pos(spinner, 0, -100);
  lv_spinner_set_anim_params(spinner, 500, 180);

  hide(loadingSpinner);
}

void setSsidOptions() {
  auto ssids = getNetworks();
  // //LOG_DEBUG(LABEL, "Networks: " << ssids);

  String options = "";
  for (int i = 0; i < ssids->size() - 1; i++) {
    options += ssids->at(i) + "\n";
  }
  options += ssids->at(ssids->size() - 1);
  LOG_DEBUG(LABEL, options);
  //  return;

  lv_dropdown_set_options(ssidDropdown, options.c_str());
  lv_dropdown_set_selected(ssidDropdown, 1);
  lv_dropdown_set_text(ssidDropdown, "");
  // enable ssid and password
  enable(ssidDropdown);
  lv_obj_set_style_bg_color(ssidDropdown, lv_color_white(), 0);
}

void checkScan(lv_timer_t *timer) {
  // //LOG_DEBUG(LABEL, "Checking scan");
  if (checkWifiScan()) {
    return;
  }

  LOG_DEBUG(LABEL, "Scan complete");
  lv_timer_delete(timer);
  setSsidOptions();
}

void startScan() {
  // initiate scan
  // lv_label_set_text(statusLabel, "Scanning")
  // sleep(1);
  LOG_INFO(LABEL, "Starting scan task");
  lv_dropdown_set_text(ssidDropdown, "Scanning...");
  initWifi();
  scanNetworks();
  lv_timer_create(checkScan, 20, NULL);
}

void checkWifi() {
  auto ssid = readFromStorage(WIFI_SSID_KEY);
  if (!ssid || ssid == "") {
    return;
  }

  auto password = readFromStorage(WIFI_PASSWORD_KEY);
  if (!password || password == "") {
    return;
  }

  tryConnectToWifi(ssid, password);
}

void buildWifiScreen() {
  buildTitle();
  buildSsidInput();
  buildPasswordInput();
  buildConnectButton();
  buildConnectionStatus();

  makeKeyboard();
  buildSpinner();

  // startScan();
}
