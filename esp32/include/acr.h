#pragma once

using namespace std;

struct SongInfo {
  string artist;
  string song;
  string album;
};

SongInfo identifySongV2(uint8_t* wavFile, int size, bool local = false);