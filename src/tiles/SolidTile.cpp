#include "SolidTile.h"

SolidTile::SolidTile(sf::Sprite sprite) : Tile{sprite} {}

bool SolidTile::isSolidFor(GameState*, size_t, size_t, size_t, Tile*) const {
  return true;
}

int SolidTile::getZLayer(GameState*, size_t, size_t, size_t) const { return 5; }
