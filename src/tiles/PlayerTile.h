#pragma once

#include "Tile.h"

class PlayerTile : public Tile {
 private:
  size_t moveToX;
  size_t moveToY;
  sf::Time startedMoving;

 public:
  PlayerTile(sf::Sprite sprite, size_t x, size_t y);

  bool update(GameState* state, size_t x, size_t y) override;
  void render(GameState* state, size_t x, size_t y) override;
  int getZLayer(GameState* state, size_t x, size_t y) const override;
};
