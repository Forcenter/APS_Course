#include "StatisticsGatherer.h"

#include <iterator>

void StatisticsGatherer::append(const BufSetCycleSummary& sum) {
  applicationsReceived += sum.applicationsReceived;
  applicationsDenied += sum.applicationsDenied;
  connectionAmount += sum.newConnections;

  for (auto& it: sum.applicationsReceivedPerSender) {
    auto temp = senderSummary.find(it);
    if (temp == senderSummary.end()) {
      senderSummary.insert({ it, {} });
      ++senderSummary.find(it)->second.applicationsReceived;
    }
    else
      ++temp->second.applicationsReceived;
  }

  for (auto& it: sum.applicationsDeniedPerSender) {
    auto temp = senderSummary.find(it);
    if (temp == senderSummary.end()) {
      senderSummary.insert({ temp->first, {} });
      ++senderSummary.rbegin()->second.applicationsDenied;
    }
    else
      ++temp->second.applicationsDenied;
  }

  deltaApplicationsReceived.push_back(sum.applicationsReceived);
  deltaApplicationsDenied.push_back(sum.applicationsDenied);
  deltaConnectionAmount.push_back(sum.newConnections);
}

void StatisticsGatherer::append(const BufGetCycleSummary& sum) {
  worksStarted += sum.worksStarted;
  worksFinished += sum.worksFinished;
  for (auto& it: sum.generatedPackageSizes) {
    auto temp = senderSummary.find(it.first);
    temp->second.generatedPackageSizes.push_back(it.second);
    generatedPackageSizes.push_back(it.second);
  }

  deltaWorksStarted.push_back(sum.worksStarted);
  deltaWorksFinished.push_back(sum.worksFinished);
  for (auto& it: sum.workingEngineers) {
    bool counted = false;
    if (it.second) {
      for (auto& jt : workTime) {
        if (it.first == jt.first) {
          ++jt.second;
          counted = true;
          break;
        }
      }
      if (!counted)
        workTime.push_back({ it.first, 1 });
    }
  }
}

void StatisticsGatherer::append(const BufferStateSummary& sum) {
  bufferFullness.push_back(sum.bufferFullness);
  auto it = senderSummary.begin();
  for (auto& itSum: sum.bufferPerSender) {
    if (it == senderSummary.end()) {
      senderSummary.insert({ itSum.first, {} });
      senderSummary.rbegin()->second.bufferFullness.push_back(itSum.second);
      senderSummary.rbegin()->second.firstIterationInBuffer = iterationCounter;
      --it;
    }
    else {
      it->second.bufferFullness.push_back(itSum.second);
      if (it->second.firstIterationInBuffer == 0)
        it->second.firstIterationInBuffer = iterationCounter;
      ++it;
    }
  }

}

void StatisticsGatherer::nextIteration() {
  ++iterationCounter;
}

unsigned int StatisticsGatherer::getAppReceived() {
  return applicationsReceived;
}

unsigned int StatisticsGatherer::getAppDenied() {
  return applicationsDenied;
}

unsigned int StatisticsGatherer::getConnections() {
  return connectionAmount;
}

unsigned int StatisticsGatherer::getWorksStarted() {
  return worksStarted;
}

unsigned int StatisticsGatherer::getWorksFinished() {
  return worksFinished;
}

const std::vector< unsigned int >& StatisticsGatherer::getDeltaAppReceived() {
  return deltaApplicationsReceived;
}

const std::vector< unsigned int >& StatisticsGatherer::getDeltaAppDenied() {
  return deltaApplicationsDenied;
}

const std::vector< unsigned int >& StatisticsGatherer::getDeltaConnections() {
  return deltaConnectionAmount;
}

const std::vector< unsigned int >& StatisticsGatherer::getDeltaWorksStarted() {
  return deltaWorksStarted;
}

const std::vector< unsigned int >& StatisticsGatherer::getDeltaWorksFinished() {
  return deltaWorksFinished;
}

const std::vector< unsigned int >& StatisticsGatherer::getBufferAmountFilled() {
  return bufferFullness;
}

const std::vector< unsigned int >& StatisticsGatherer::getPackageSizes() {
  return generatedPackageSizes;
}

const std::vector< std::pair< unsigned int, unsigned int > >& StatisticsGatherer::getWorkTime() {
  return workTime;
}

const std::map<unsigned int, SenderSummaryInformation>& StatisticsGatherer::getSenderSummary() {
  return senderSummary;
}
