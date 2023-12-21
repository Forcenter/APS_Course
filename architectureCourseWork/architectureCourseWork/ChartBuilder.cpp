#include "ChartBuilder.h"

#include <functional>

ChartBuilder::ChartBuilder(const std::vector< unsigned int >& valueTable, int id, const std::string& name):
  window(sf::VideoMode(960, 510), name)
{
  sf::Vector2i pos = { id % 2, id / 2 };
  pos.x *= window.getSize().x;
  pos.y *= window.getSize().y + 25;
  window.setPosition(pos);
  window.display();

  //We use one point and draw it multiple times to draw the chart
  sf::CircleShape pt;
  pt.setRadius(3);
  pt.setFillColor(sf::Color::Red);
  window.clear(sf::Color::White);

  float coefX = static_cast< float >(window.getSize().x - 50) / valueTable.size();
  float max = *std::max_element(valueTable.begin(), valueTable.end());
  float coefY = static_cast< float >(window.getSize().y - 50) / max;
  
  int x = -1;
  for (auto& it: valueTable) {
    ++x;
    pt.setPosition(sf::Vector2f{ x * coefX + 25, window.getSize().y - (static_cast< float >(it) * coefY + 25) });
    window.draw(pt);
  }
  window.display();
}

void ChartBuilder::update() {
  sf::Event e;
  while (window.pollEvent(e)) {
    if (e.type == sf::Event::Closed)
      window.close();
  }
}
