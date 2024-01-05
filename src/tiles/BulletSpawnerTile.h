#pragma once

#include <functional>

#include "Tile.h"

template <typename Bullet>
class BulletSpawnerTile : public Tile {
 private:
  int spawnDelayMax;
  int spawnDelay;
  std::function<Bullet *(GameState *state, size_t x, size_t y)> spawner;

 public:
  BulletSpawnerTile(
      sf::Sprite sprite, int spawnDelay,
      std::function<Bullet *(GameState *state, size_t x, size_t y)>);

  int getZLayer(GameState *state, size_t x, size_t y) const override;
  void prepareMove(GameState *state, size_t x, size_t y) override;
};

#include "BulletSpawnerTile.hpp"
