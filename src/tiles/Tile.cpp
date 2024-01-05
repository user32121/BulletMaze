#include "Tile.h"

Tile::Tile(sf::Sprite sprite) : sprite{sprite} {}

void Tile::render(GameState* state, size_t x, size_t y) {
  sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
  state->window->draw(sprite);
}

int Tile::getZLayer(GameState* state, size_t x, size_t y) const { return 0; }

bool Tile::update(GameState* state, size_t x, size_t y) { return false; }

void Tile::prepareMoveTile(GameState* state, size_t x, size_t y) {}
