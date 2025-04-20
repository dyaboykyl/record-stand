
#include <lvgl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

static lv_color_t BASE_ONE = lv_color_hex(0x815432);
static lv_color_t BASE_ONE_GRAD = lv_color_hex(0x562d0f);
static lv_color_t BASE_TWO = lv_color_hex(0xcea578);
static lv_color_t DISABLED_COLOR = lv_color_make(200, 200, 200);

inline void hide(lv_obj_t *obj) { lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN); }
inline void show(lv_obj_t *obj) { lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN); }
inline void enable(lv_obj_t *obj) {
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}
inline void disable(lv_obj_t *obj) {
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}