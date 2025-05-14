#pragma once

#include "AppState.h"

using namespace std;

SongInfo identifySongV2(uint8_t* wavFile, int size, bool local = false);
