#pragma once

#include "Tile.h"

/// @brief Contains logic for player movement
class PlayerTile : public Tile {
 private:
  size_t moveToX;
  size_t moveToY;
  sf::Time startedMoving;

 public:
  PlayerTile(sf::Sprite sprite, size_t x, size_t y);

  void update(GameState* state, size_t x, size_t y, size_t i) override;
  void render(GameState* state, size_t x, size_t y, size_t i) override;
  int getZLayer(GameState* state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState* state, size_t x, size_t y, size_t i) override;
  bool finishMove(GameState* state, size_t x, size_t y, size_t i) override;
};
