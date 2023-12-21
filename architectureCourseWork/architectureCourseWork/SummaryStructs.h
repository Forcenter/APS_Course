#ifndef SUMMARY_STRUCTS_H
#define SUMMARY_STRUCTS_H

#include <vector>
#include <list>
#include <map>

struct SenderSummaryInformation {
  unsigned int applicationsReceived = 0;
  unsigned int applicationsDenied = 0;
  unsigned int firstIterationInBuffer = 0;
  std::vector< unsigned int > generatedPackageSizes;
  std::vector< unsigned int > bufferFullness;
};

struct BufSetCycleSummary {
  unsigned int applicationsReceived;
  unsigned int applicationsDenied;
  unsigned int newConnections;
  std::list< unsigned int > applicationsReceivedPerSender;
  std::list< unsigned int > applicationsDeniedPerSender;
};

struct BufGetCycleSummary {
  unsigned int worksStarted;
  unsigned int worksFinished;
  std::list< std::pair< unsigned int, bool > > workingEngineers;
  std::list< std::pair< unsigned int, unsigned int > > generatedPackageSizes;
};

struct BufferStateSummary {
  unsigned int bufferFullness;
  const std::map< unsigned int, unsigned int >& bufferPerSender;
};

#endif