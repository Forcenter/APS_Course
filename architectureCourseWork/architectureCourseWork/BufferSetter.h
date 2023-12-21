#ifndef BUFFER_SETTER_H
#define BUFFER_SETTER_H

#include "SFML/Network.hpp"
#include <vector>
#include <string>
#include <queue>
#include <memory>

#include "Application.h"
#include "Buffer.h"
#include "SummaryStructs.h"

class BufferSetter {
private:
  sf::TcpListener listener;

  std::vector< std::unique_ptr< sf::TcpSocket > > sources;
  Buffer& buffer;
  std::deque< Application >::iterator bufIter;

  BufSetCycleSummary resultGatherer;

  bool checkNewConnections();

  bool insertApplicationsIntoBuffer();

public:
  BufferSetter(unsigned int port, Buffer& buffer);

  BufSetCycleSummary update();
};

#endif