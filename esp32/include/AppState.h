#pragma once

#include <lvgl.h>

#include "Observable.h"
#include "utils.h"

using namespace std;

#define NO_RESULT "NO_RESULT"

enum Screen {
  CALIBRATION,
  MAIN,
  SETTINGS,
};

struct IdentifyResult {
  string song;
  string artist;
  string album;
  string error;
};

struct SongInfo {
  string artist;
  string song;
  string album;

  bool operator!=(const SongInfo& other) const {
    return song != other.song         //
           || artist != other.artist  //
           || album != other.album;   //
  }
};

class AppState {
 public:
  Observable<Pair> screenOffset;
  Observable<SongInfo> songInfo;
  Observable<string> identifyError;
  Observable<Screen> activeScreen;

  AppState()
      : screenOffset({0, 0}), songInfo({"A long song", "A long artist", ""}), identifyError("") {}

  void init();
  void goToScreen(Screen screen);
  void onNewIdentifyResult(const char* result);

 private:
  lv_obj_t* activeScreenParent;

  void loadOffset();
  void applyCalibration();
};
