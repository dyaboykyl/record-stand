#pragma once

#include <tuple>

#include "Observable.h"

using namespace std;

using Pair = tuple<int, int>;

class ScreenController {
 public:
  Observable<tuple<int, int>> screenOffset;

  ScreenController() : screenOffset({0, 0}) {}

  void init();

 private:
  void loadOffset();
};