#include "Engineer.h"

void Engineer::giveWork(const Application& app, int time) {
  workedApplication = app;
  workTime = time;
}

Engineer::Engineer():
  workedApplication("", "", "", 0)
{}

bool Engineer::update() {
  if (workTime > 0) {
    --workTime;
    if (workTime == 0)
      justFinished = true;
  }
  return true;
}

bool Engineer::isWorking() {
  return workTime != 0;
}

bool Engineer::hasJustFinished() {
  if (justFinished) {
    justFinished = false;
    return true;
  }
  return false;
}
