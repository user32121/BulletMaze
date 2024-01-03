#include "tile.h"

tile::tile(sf::Sprite sprite) : sprite{sprite} {}

void tile::render(sf::RenderWindow* window, size_t x, size_t y) {
  sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
  window->draw(sprite);
}
