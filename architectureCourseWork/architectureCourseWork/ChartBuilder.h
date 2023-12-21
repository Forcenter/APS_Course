#ifndef CHART_BUILDER_H
#define CHART_BUILDER_H

#include "SFML/Graphics.hpp"

class ChartBuilder {
private:
  sf::RenderWindow window;

public:
  ChartBuilder(const std::vector< unsigned int >& valueTable, int id, const std::string& name);

  void update();
};

#endif