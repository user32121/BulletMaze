#pragma once

#include <functional>

#include "Tile.h"

/// @brief Spawns a Bullet periodically
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

  int getZLayer(GameState *state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState *state, size_t x, size_t y, size_t i) override;
};

#include "BulletSpawnerTile.hpp"
