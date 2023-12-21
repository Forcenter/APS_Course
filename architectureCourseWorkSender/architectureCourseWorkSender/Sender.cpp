#include "Sender.h"

#include <fstream>
#include <iostream>

Application Sender::generateApplication() {
  return Application("Infinity", "Fuel ramp", "A113BB", id);
}

bool Sender::sendApplication(const Application& app) {
  sf::Packet packet;
  packet << app.getCarBrandName() << app.getBrokenPart() << app.getCarNumber() << app.getSourceId();

  int counter = 0;
  while (socket.send(packet) != sf::Socket::Done) {
    ++counter;
    if (counter > 500)
      return false;
  }
  return true;
}

Sender::Sender(const std::string& templateFile, unsigned int id) :
  id(id)
{
  std::ifstream fin;
  if (!templateFile.empty())
    fin.open(templateFile);

  socket.connect(sf::IpAddress::getLocalAddress(), 1024, sf::seconds(100));
}

bool Sender::generateAndSend() {
  return sendApplication(generateApplication());
}
