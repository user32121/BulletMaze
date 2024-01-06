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
  virtual void update(GameState* state, size_t x, size_t y, size_t i);

  /// @brief Prepares actions that occur when a player moves
  virtual void prepareMove(GameState* state, size_t x, size_t y, size_t i);

  /// @brief Performs an updates to board structure after this tile has finished
  /// its action
  /// @return True if this tile has moved (and thus an iterator into board[x][y]
  /// would be invalidated)
  virtual bool finishMove(GameState* state, size_t x, size_t y, size_t i);

  /// @brief Draws the tile at the given coordinates
  virtual void render(GameState* state, size_t x, size_t y, size_t i);

  /// @brief the z order used during rendering
  virtual int getZLayer(GameState* state, size_t x, size_t y, size_t i) const;

  /// @brief True if other should treat this as an impassable tile
  virtual bool isSolidFor(GameState* state, size_t x, size_t y, size_t i,
                          Tile* other) const;

 protected:
  sf::Sprite sprite;
};
