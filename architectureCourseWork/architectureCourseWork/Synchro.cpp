#include "Synchro.h"

#include <iostream>

Synchro::Synchro(unsigned int sourceAmount, unsigned int port) {
  listener.listen(port);
  listener.setBlocking(false);

  std::unique_ptr< sf::TcpSocket > socket;
  bool hasCreatedSocket = false;
  while (sockets.size() < sourceAmount) {
    if (!hasCreatedSocket) {
      socket = std::make_unique< sf::TcpSocket >();
      socket->setBlocking(false);
      hasCreatedSocket = true;
    }
    if (listener.accept(*socket) == sf::Socket::Done) {
      sockets.push_back(std::move(socket));
      hasCreatedSocket = false;
    }
  }
  std::cout << "Synchro is ready to work\n";
}

void Synchro::letThemWork(){
  sf::Packet packet;

  if (!waitThem)
    return;

  for (auto& it: sockets)
    while (it->send(packet) != sf::Socket::Done) {}

  packet.clear();

  for (auto& it: sockets) {
    while (it->receive(packet) != sf::Socket::Done) {}
    int a;
    packet >> a;
    if (a == 1)
      waitThem = false;
  }
}
