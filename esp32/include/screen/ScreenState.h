#pragma once

#include <lvgl.h>

#include "Observable.h"
#include "utils.h"

using namespace std;

enum Screen {
  CALIBRATION,
  MAIN,
  SETTINGS,
};

class ScreenState {
 public:
  Observable<Pair> screenOffset;

  ScreenState() : screenOffset({0, 0}) {}

  void init();
  void goToScreen(Screen screen);

 private:
  Screen activeScreenType;
  lv_obj_t* activeScreen;

  void loadOffset();
  void applyCalibration();
};
