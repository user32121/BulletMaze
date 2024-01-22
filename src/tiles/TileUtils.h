#pragma once

#include "Tile.h"

class Tile;
struct GameState;
struct SerializedTileData {
  sf::Vector2i v2i, v2i2;
  int i, i2;
  Tile* (*fp)(GameState*, size_t, size_t);
  nlohmann::json json;
};

struct SerializedTile {
  /// @brief a function to deserialize the data back into a Tile
  Tile* (*restore)(GameState* state, size_t x, size_t y, size_t i,
                   SerializedTileData* data);
  SerializedTileData data;
};

Tile* jsonToTile(GameState* state, nlohmann::json* json, size_t x, size_t y,
                 size_t i);
