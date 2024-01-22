#pragma once

#include "Tile.h"

/// @brief Behaves the same as a Tile, but is solid
class SolidTile : public Tile {
 private:
 public:
  SolidTile(sf::Sprite sprite);
  SolidTile(GameState* state, nlohmann::json* json, size_t x, size_t y,
            size_t i);

  bool isSolidFor(GameState* state, size_t x, size_t y, size_t i,
                  Tile* other) const override;
  int getZLayer(GameState* state, size_t x, size_t y, size_t i) const override;
  SerializedTile serialize() const override;
};