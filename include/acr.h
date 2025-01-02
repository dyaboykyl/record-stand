#pragma once

struct SongInfo {
    String artist;
    String song;
    String album;
};

SongInfo identifySongV2(uint8_t* wavFile, int size, bool local = false);