#pragma once

#include <tuple>

#include "Observable.h"

using namespace std;

using Pair = tuple<int, int>;

class ScreenState {
 public:
  Observable<tuple<int, int>> screenOffset;

  ScreenState() : screenOffset({0, 0}) {}

  void init();

 private:
  void loadOffset();
};