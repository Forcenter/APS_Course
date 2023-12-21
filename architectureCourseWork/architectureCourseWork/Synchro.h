#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <vector>
#include <memory>
#include "SFML/Network.hpp"

class Synchro {
private:
  sf::TcpListener listener;
  std::vector< std::unique_ptr <sf::TcpSocket > > sockets;

  bool waitThem = true;

public:
  Synchro(unsigned int sourceAmount, unsigned int port);

  void letThemWork();
};

#endif