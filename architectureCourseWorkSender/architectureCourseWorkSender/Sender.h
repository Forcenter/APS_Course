#ifndef SENDER_H
#define SENDER_H

#include <vector>
#include <string>
#include "SFML/Network.hpp"

#include "Application.h"

class Sender {
private:
  unsigned int id;
  sf::TcpSocket socket;
  std::vector< std::string > carBrandNamesArray;
  std::vector< std::string > brokenPartsArray;
  std::vector< std::string > carNumberArray;

  Application generateApplication();
  bool sendApplication(const Application& app);

public:
  Sender(const std::string& templateFile, unsigned int id);

  bool generateAndSend();
};

#endif SENDER_H