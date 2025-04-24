#pragma once

#include <cstdint>

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class Canvas {
 public:
  Canvas(uint16_t width, uint16_t height);
  ~Canvas();

  void drawCircle(int x, int y, int radius, RGB color);
  void drawLine(int x1, int y1, int x2, int y2, RGB color);

 private:
  uint16_t _width;
  uint16_t _height;
};
