#include "Tile.h"

Tile::Tile(sf::Sprite sprite) : sprite{sprite} {}

void Tile::render(GameState* state, size_t x, size_t y) {
  sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
  state->window->draw(sprite);
}

void Tile::update(GameState* state) {}
