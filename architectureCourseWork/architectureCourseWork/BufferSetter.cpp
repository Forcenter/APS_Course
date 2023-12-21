#include "BufferSetter.h"

#include <iostream>
#include <memory>

bool BufferSetter::checkNewConnections() {
  static std::unique_ptr< sf::TcpSocket > socket;
  static bool hasCreatedSocket = false;
  do {
    if (!hasCreatedSocket) {
      socket = std::make_unique< sf::TcpSocket >();
      socket->setBlocking(false);
      hasCreatedSocket = true;
    }
    if (listener.accept(*socket) == sf::Socket::Done) {
      sources.push_back(std::move(socket));
      ++resultGatherer.newConnections;
      hasCreatedSocket = false;
    }
  } while (!hasCreatedSocket);
  return resultGatherer.newConnections > 0;
}

bool BufferSetter::insertApplicationsIntoBuffer() {
  for (auto& it: sources) {
    sf::Packet packet;
    if (it->receive(packet) == sf::Socket::Done) {
      std::string carBrandName;
      std::string brokenPart;
      std::string carNumber;
      unsigned int sourceId;

      packet >> carBrandName >> brokenPart >> carNumber >> sourceId;

      ++resultGatherer.applicationsReceived;
      resultGatherer.applicationsReceivedPerSender.push_back(sourceId);

      Application received(carBrandName, brokenPart, carNumber, sourceId);
      std::deque< Application >::iterator temp = buffer.getFirstEmptyFrom(bufIter);
      if (temp == buffer.getIterEnd()) {
        temp = bufIter;
        ++resultGatherer.applicationsDenied;
        resultGatherer.applicationsDeniedPerSender.push_back(sourceId);
      }
      buffer.pushApplicationTo(temp, received);
    }
  }
  return resultGatherer.applicationsReceived > 0;
}

BufferSetter::BufferSetter(unsigned int port, Buffer& buffer):
  buffer(buffer)
{
  listener.listen(port);
  listener.setBlocking(false);

  bufIter = buffer.getIterBegin();
}

BufSetCycleSummary BufferSetter::update() {
  resultGatherer = BufSetCycleSummary{};

  checkNewConnections();

  insertApplicationsIntoBuffer();

  return resultGatherer;
}
