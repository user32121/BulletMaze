#include <SFML/Graphics.hpp>

#define TILE_SIZE 64

class tile {
 public:
  tile(sf::Sprite sprite);

  virtual void render(sf::RenderWindow* window, size_t x, size_t y);

 private:
  sf::Sprite sprite;
};
