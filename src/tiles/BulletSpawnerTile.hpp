#pragma once

#include "BulletSpawnerTile.h"

template <typename Bullet>
inline BulletSpawnerTile<Bullet>::BulletSpawnerTile(
    sf::Sprite sprite, int spawnPeriod,
    std::function<Bullet *(GameState *state, size_t x, size_t y)> spawn)
    : BulletSpawnerTile{sprite, spawnPeriod, 0, spawn} {}

template <typename Bullet>
inline BulletSpawnerTile<Bullet>::BulletSpawnerTile(
    sf::Sprite sprite, int spawnPeriod, int initialSpawnDelay,
    std::function<Bullet *(GameState *state, size_t x, size_t y)> spawn)
    : Tile{sprite},
      spawnDelayMax{spawnPeriod},
      spawnDelay{initialSpawnDelay},
      spawner{spawn} {}

template <typename Bullet>
inline int BulletSpawnerTile<Bullet>::getZLayer(GameState *, size_t, size_t,
                                                size_t) const {
  return 10;
}

template <typename Bullet>
inline void BulletSpawnerTile<Bullet>::prepareMove(GameState *state, size_t x,
                                                   size_t y, size_t) {
  --spawnDelay;
  if (spawnDelay <= 0) {
    spawnDelay = spawnDelayMax;
    state->board[x][y].push_back(spawner(state, x, y));
  }
}
