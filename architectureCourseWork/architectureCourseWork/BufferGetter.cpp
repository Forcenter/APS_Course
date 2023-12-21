#include "BufferGetter.h"

#include <queue>
#include <iostream>
#include <random>

bool BufferGetter::generateNewPackage() {
  static int counter = 0;

  unsigned int packageSourceId = buffer.getMinSourceId();
  if (!packageSourceId)
    return false;

  currentPackage.clear();
  auto bufIter = buffer.getIterBegin();

  while ((bufIter = buffer.getFirstAppFromSource(packageSourceId, bufIter)) != buffer.getIterEnd())
    currentPackage.push_back(buffer.pullApplicationAt(bufIter));

  resultGatherer.generatedPackageSizes.push_back({ packageSourceId, currentPackage.size() });

  return true;
}

void BufferGetter::giveWork() {
  for (auto& it: engineers) {
    if (!it.isWorking()) {
      ++resultGatherer.worksStarted;
      it.giveWork(*currentPackage.begin(), rand() % 4 + workTime - 2);
      currentPackage.pop_front();
      if (currentPackage.empty()) {
        if (!generateNewPackage())
          return;
      }
    }
  }
}

BufferGetter::BufferGetter(unsigned int engineersAmount, unsigned int workTime, Buffer& buffer):
  buffer(buffer),
  workTime(workTime)
{
  for (int i = 0; i < engineersAmount; ++i) {
    Engineer temp;
    engineers.push_back(Engineer());
  }
}

BufGetCycleSummary BufferGetter::update() {
  resultGatherer = {};
  
  if (currentPackage.empty())
    generateNewPackage();

  for (int i = 0; i < engineers.size(); ++i) {
    engineers[i].update();
    if (engineers[i].hasJustFinished())
      ++resultGatherer.worksFinished;
    if (engineers[i].isWorking())
      resultGatherer.workingEngineers.push_back({ i, true });
    else
      resultGatherer.workingEngineers.push_back({ i, false });
  }

  if (!currentPackage.empty())
    giveWork();

  return resultGatherer;
}
