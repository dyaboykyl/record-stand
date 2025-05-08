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

#include <tuple>

#include "Observable.h"
#include "logging.h"
#include "storage.h"
#include "utils.h"

using namespace std;

static auto logger = Logger("ScreenState");

void ScreenState::init() {
  logger.info("[init]");
  loadOffset();
  screenOffset.subscribe(
      [this](const Pair& offset) { writeToStorage(SCREEN_OFFSET, pairToString(offset)); });
}

void ScreenState::loadOffset() {
  auto offsetString = readFromStorage(SCREEN_OFFSET);
  screenOffset.set(pairFromString(offsetString));
}
