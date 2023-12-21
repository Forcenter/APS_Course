#ifndef STATISTICS_GATHERER_H
#define STATISTICS_GATHERER_H

#include <vector>
#include <map>

#include "SummaryStructs.h"

class StatisticsGatherer {
private:
  unsigned int iterationCounter = 0;

  unsigned int applicationsReceived;
  unsigned int applicationsDenied;
  unsigned int connectionAmount;
  unsigned int worksStarted;
  unsigned int worksFinished;

  std::vector< unsigned int > deltaApplicationsReceived;
  std::vector< unsigned int > deltaApplicationsDenied;
  std::vector< unsigned int > deltaConnectionAmount;
  std::vector< unsigned int > deltaWorksStarted;
  std::vector< unsigned int > deltaWorksFinished;
  std::vector< std::pair< unsigned int, unsigned int > > workTime;

  std::vector< unsigned int > bufferFullness;

  std::vector< unsigned int > generatedPackageSizes;

  std::map< unsigned int, SenderSummaryInformation > senderSummary;

public:
  void append(const BufSetCycleSummary& sum);
  void append(const BufGetCycleSummary& sum);
  void append(const BufferStateSummary& sum);

  void nextIteration();

  unsigned int getAppReceived();
  unsigned int getAppDenied();
  unsigned int getConnections();
  unsigned int getWorksStarted();
  unsigned int getWorksFinished();

  const std::vector< unsigned int >& getDeltaAppReceived();
  const std::vector< unsigned int >& getDeltaAppDenied();
  const std::vector< unsigned int >& getDeltaConnections();
  const std::vector< unsigned int >& getDeltaWorksStarted();
  const std::vector< unsigned int >& getDeltaWorksFinished();

  const std::vector< unsigned int >&  getBufferAmountFilled();
  const std::vector< unsigned int >& getPackageSizes();
  const std::vector< std::pair< unsigned int, unsigned int > >& getWorkTime();

  const std::map< unsigned int, SenderSummaryInformation >& getSenderSummary();
};

#endif