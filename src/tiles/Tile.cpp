#include "Tile.h"

Tile::Tile(sf::Sprite sprite) : sprite{sprite} {}

void Tile::render(GameState* state, size_t x, size_t y) {
  sprite.setPosition(float(x * TILE_SIZE), float(y * TILE_SIZE));
  state->window->draw(sprite);
}

int Tile::getZLayer(GameState*, size_t, size_t) const { return 0; }

bool Tile::isSolidFor(GameState*, size_t, size_t, Tile*) const { return false; }

bool Tile::update(GameState*, size_t, size_t) { return false; }

void Tile::prepareMove(GameState*, size_t, size_t) {}
