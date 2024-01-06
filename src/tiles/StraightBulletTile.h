#pragma once

#include "Tile.h"

enum DIRECTION { LEFT, RIGHT, UP, DOWN };

/// @brief A bullet that moves in a straight line
class StraightBulletTile : public Tile {
 private:
  DIRECTION dir;
  size_t moveToX;
  size_t moveToY;
  bool collided = false;
  int value;

 public:
  StraightBulletTile(sf::Sprite sprite, size_t x, size_t y, DIRECTION dir,
                     int value = 1);

  int getZLayer(GameState* state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState* state, size_t x, size_t y, size_t i) override;
  bool finishMove(GameState* state, size_t x, size_t y, size_t i) override;
  void render(GameState* state, size_t x, size_t y, size_t i) override;
  int getBulletValue(GameState* state, size_t x, size_t y,
                     size_t i) const override;
};
