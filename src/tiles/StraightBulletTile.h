#pragma once

#include "Tile.h"

enum DIRECTION { LEFT, RIGHT, UP, DOWN };

class StraightBulletTile : public Tile {
 private:
  DIRECTION dir;

 public:
  StraightBulletTile(sf::Sprite sprite, size_t x, size_t y, DIRECTION dir);

  int getZLayer(GameState* state, size_t x, size_t y) const override;
  // TODO
  //  bool update(GameState* state, size_t x, size_t y) override;
  //  void prepareMove(GameState* state, size_t x, size_t y) override;
  //  void render(GameState* state, size_t x, size_t y) override;
};
