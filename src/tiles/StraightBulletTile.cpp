#include "StraightBulletTile.h"

StraightBulletTile::StraightBulletTile(sf::Sprite sprite, size_t x, size_t y,
                                       DIRECTION dir)
    : Tile{sprite}, dir{dir} {}

int StraightBulletTile::getZLayer(GameState* state, size_t x, size_t y) const {
  return 20;
}
