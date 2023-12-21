#ifndef BUFFER_GETTER_H
#define BUFFER_GETTER_H

#include <vector>
#include <list>

#include "Application.h"
#include "Engineer.h"
#include "SummaryStructs.h"
#include "Buffer.h"

class BufferGetter {
private:
  Buffer& buffer;
  std::list< Application > currentPackage;
  std::vector< Engineer > engineers;

  unsigned int workTime;

  BufGetCycleSummary resultGatherer;

  bool generateNewPackage();
  void giveWork();

public:
  BufferGetter(unsigned int engineersAmount, unsigned int workTime, Buffer& buffer);

  BufGetCycleSummary update();
};

#endif