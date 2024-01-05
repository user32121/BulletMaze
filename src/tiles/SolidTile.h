#include "Tile.h"

/// @brief Behaves the same as a Tile, but this one is solid
class SolidTile : public Tile {
 private:
 public:
  SolidTile(sf::Sprite sprite);

  bool isSolidFor(GameState* state, size_t x, size_t y,
                  Tile* other) const override;
};