#include "SolidTile.h"

SolidTile::SolidTile(sf::Sprite sprite) : Tile{sprite} {}

bool SolidTile::isSolidFor(GameState* state, size_t x, size_t y,
                           Tile* other) const {
  return true;
}

int SolidTile::getZLayer(GameState* state, size_t x, size_t y) const {
  return 5;
}
