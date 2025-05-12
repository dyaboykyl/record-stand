/**
 * ### App Controller
#### State
- Active screen
 - subsription: lvgl.updateScreen
- Screen offset
  Subscription: storage.save
#### Actions
- updateOffset(hor, ver):
    offsetObservable.update(hor, ver)
    lvgl_screen_offset(lvgl_active_screen())
#### init
 - Load offset
 - Draw screen

### Calibration Screen
#### State
- previous offset
#### Components
- Calibration lines
- 4 arrow buttons
- Confirm
- Cancel
- Title
- Offset
  - subscribe: screenController.offset

#### Controller
- onDirectionPressed
 - appController.updateOffset
- onDonePressed(bool confirm)
 - if (!confirm):
    appController.updateOffset(previousOffset)
 */

// #include <observable/observable.hpp>
#include "screen/ScreenState.h"

#include <Arduino.h>
#include <esp_log.h>

#include "Observable.h"
#include "logging.h"
#include "screen/calibration_screen.h"
#include "screen/main_screen.h"
#include "screen/settings_screen.h"
#include "storage.h"
#include "utils.h"

using namespace std;

static auto logger = Logger("ScreenState");

void ScreenState::init() {
  logger.info("[init]");
  loadOffset();
  screenOffset.subscribe([this](const Pair& offset) {
    writeToStorage(SCREEN_OFFSET, pairToString(offset));
    applyCalibration();
  });
}

void ScreenState::goToScreen(Screen screenType) {
  if (screenType == activeScreenType) {
    return;
  }

  lv_obj_t* nextScreen;
  auto animation = LV_SCR_LOAD_ANIM_NONE;
  switch (screenType) {
    case CALIBRATION:
      nextScreen = buildCalibrationScreen();
      animation = LV_SCR_LOAD_ANIM_MOVE_TOP;
      break;
    case MAIN:
      nextScreen = buildMainScreen();
      animation =
          activeScreenType == Screen::SETTINGS ? LV_SCR_LOAD_ANIM_MOVE_TOP : LV_SCR_LOAD_ANIM_NONE;
      break;
    case SETTINGS:
      nextScreen = buildSettingsScreen();
      animation = activeScreenType == Screen::MAIN ? LV_SCR_LOAD_ANIM_MOVE_BOTTOM
                                                   : LV_SCR_LOAD_ANIM_MOVE_TOP;
      break;
  }
  lv_screen_load_anim(activeScreen, animation, 250, 0, false);
}

void ScreenState::loadOffset() {
  auto offsetString = readFromStorage(SCREEN_OFFSET);
  screenOffset.set(pairFromString(offsetString));
}

void ScreenState::applyCalibration() {
  auto offset = screenOffset.get();
  lv_obj_set_style_translate_x(activeScreen, offset.x, 0);
  lv_obj_set_style_translate_y(activeScreen, offset.y, 0);
}
