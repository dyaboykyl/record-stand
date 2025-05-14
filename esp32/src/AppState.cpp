#include "AppState.h"

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

static auto logger = Logger("AppState");

void AppState::init() {
  logger.info("[init]");
  loadOffset();

  screenOffset.subscribe([this](const Pair& offset) {
    writeToStorage(SCREEN_OFFSET, pairToString(offset));
    applyCalibration();
  });

  activeScreen.subscribe([this](const Screen& screen) { applyCalibration(); });
}

void AppState::goToScreen(Screen screen) {
  logger.info("[goToScreen] %d", screen);
  if (activeScreen.get() == screen) {
    return;
  }

  auto animation = LV_SCR_LOAD_ANIM_NONE;
  switch (screen) {
    case CALIBRATION:
      animation = LV_SCR_LOAD_ANIM_MOVE_BOTTOM;
      activeScreenParent = loadCalibrationScreen(animation);
      break;
    case MAIN:
      animation = activeScreen.get() == Screen::SETTINGS ? LV_SCR_LOAD_ANIM_MOVE_TOP
                                                         : LV_SCR_LOAD_ANIM_NONE;
      activeScreenParent = loadMainScreen(animation);
      break;
    case SETTINGS:
      animation = activeScreen.get() == Screen::MAIN ? LV_SCR_LOAD_ANIM_MOVE_BOTTOM
                                                     : LV_SCR_LOAD_ANIM_MOVE_TOP;
      activeScreenParent = loadSettingsScreen(animation);
      break;
  }
  activeScreen.set(screen);
}

void AppState::loadOffset() {
  auto offsetString = readFromStorage(SCREEN_OFFSET);
  screenOffset.set(pairFromString(offsetString));
}

void AppState::applyCalibration() {
  auto offset = screenOffset.get();
  lv_obj_set_style_translate_x(activeScreenParent, offset.x, 0);
  lv_obj_set_style_translate_y(activeScreenParent, offset.y, 0);
}

void AppState::onNewIdentifyResult(const char* resultString) {
  auto data = split(resultString, "+=+");
  if (data[3] == NO_RESULT) {
    songInfo.set({string(data[0]), string(data[1]), string(data[2])});
    logger.info("Song: %s - %s", songInfo.get().artist.c_str(), songInfo.get().song.c_str());
  } else {
    identifyError.set(string(data[3]));
    logger.warn("Identify error: %s", identifyError.get().c_str());
  }
}