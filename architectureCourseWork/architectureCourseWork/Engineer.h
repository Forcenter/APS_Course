#ifndef ENGINEER_H
#define ENGINEER_H

#include "Application.h"

class Engineer {
private:
  Application workedApplication;
  unsigned int workTime = 0;
  bool justFinished = false;

public:
  Engineer();

  void giveWork(const Application& app, int time);

  bool update();

  bool isWorking();
  bool hasJustFinished();
};

#endif