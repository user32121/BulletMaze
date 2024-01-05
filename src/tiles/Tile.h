#pragma once

#include <SFML/Graphics.hpp>

#include "../GameState.h"

#define TILE_SIZE 64

struct GameState;
/// @brief Basic tile representing a stationary object on the board.
class Tile {
 public:
  Tile(sf::Sprite sprite);

  Tile(const Tile& other) = default;
  Tile& operator=(const Tile& other) = default;
  virtual ~Tile() = default;

  /// @brief Performs logic and calculations
  /// @return True if this tile has moved (and thus an iterator into board[x][y]
  /// would be invalidated)
  virtual bool update(GameState* state, size_t x, size_t y);
  /// @brief Sets the next position this tile will move to
  virtual void prepareMoveTile(GameState* state, size_t x, size_t y);
  /// @brief Draws the tile at the given coordinates
  virtual void render(GameState* state, size_t x, size_t y);
  virtual int getZLayer(GameState* state, size_t x, size_t y) const;

 protected:
  sf::Sprite sprite;
};
