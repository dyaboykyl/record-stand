#include <lvgl.h>

lv_obj_t *settingsScreen = nullptr;
static lv_obj_t *parent = nullptr;

lv_obj_t *buildSettingsScreen() { return parent; }