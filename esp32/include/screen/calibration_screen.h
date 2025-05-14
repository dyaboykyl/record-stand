
#include <lvgl.h>

#include "AppState.h"

class CalibrationState {
 public:
  void init();

 private:
};

lv_obj_t* loadCalibrationScreen(lv_screen_load_anim_t animation);