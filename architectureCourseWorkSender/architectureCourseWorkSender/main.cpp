#include "Sender.h"
#include <Windows.h>
#include <iostream>
#include <random>
#include <vector>

#define e 2.71828182845904523f

int random() {
  int result = 0;
  for (int i = 0; i < 9; ++i)
    result += rand() % 10 * pow(10, i);
  return result;
}

int main(int argc, char* argv[]) {
  srand(std::atoi(argv[2]));
  srand(time(NULL) + rand() % 10000);
  
  if (std::atoi(argv[2]) % 20 == 0) {
    std::string str = "Sender id ";
    str += argv[2];
    str += " started working\n";
    std::cout << str;
  }

  sf::Packet packet;
  sf::TcpSocket socket;

  socket.connect(sf::IpAddress::getLocalAddress(), 1025, sf::seconds(100));

  double randomness = 0;
  if (std::atoi(argv[1]) != 0)
    randomness = rand() % (std::atoi(argv[1]) * 2) - std::atoi(argv[1]);

  double lambda = std::atoi(argv[4]) + randomness;
  double currentProb = 1.f / pow(e, lambda);

  Sender sender("", std::atoi(argv[2]));

  for (int i = 0; i < std::atoi(argv[3]); ++i) {
    int counter = 0;
    while (socket.receive(packet) != sf::Socket::Done) {}

    double temp = static_cast< double >(random()) / 10000000000;
    if (temp < currentProb)
      sender.generateAndSend();

    currentProb *= lambda / (i + 1);

    packet.clear();
    packet << 0;

    while (socket.send(packet) != sf::Socket::Done) {}
  }
  int counter = 0;
  while (socket.receive(packet) != sf::Socket::Done) {}

  packet.clear();
  packet << 1;

  while (socket.send(packet) != sf::Socket::Done) {}

  return 0;
}