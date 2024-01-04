#pragma once

#include <SFML/Graphics.hpp>

class Tile;
#include "../game.h"

#define TILE_SIZE 64

/// @brief Basic tile representing a stationary object on the board.
class Tile {
 public:
  Tile(sf::Sprite sprite);

  Tile(const Tile& other) = default;
  Tile& operator=(const Tile& other) = default;
  virtual ~Tile() = default;

  /// @brief Draws the tile at the given coordinates
  virtual void render(GameState* state, size_t x, size_t y);
  /// @brief Performs logic and calculations
  virtual void update(GameState* state);

 private:
  sf::Sprite sprite;
};
