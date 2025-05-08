
#include "screen/ScreenController.h"

class CalibrationScreenController {
 public:
  CalibrationScreenController(ScreenController* sc) : screenController(sc) {};
  void init();

 private:
  ScreenController* screenController;
};

void buildCalibrationScreen();