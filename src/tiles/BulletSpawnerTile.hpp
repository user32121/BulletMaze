#pragma once

#include "BulletSpawnerTile.h"

template <typename Bullet>
inline BulletSpawnerTile<Bullet>::BulletSpawnerTile(
    sf::Sprite sprite, Bullet *(*spawn)(GameState *state, size_t x, size_t y),
    int spawnPeriod, int initialSpawnDelay)
    : Tile{sprite},
      spawnDelayMax{spawnPeriod},
      spawnDelay{initialSpawnDelay + 1},
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

Tile *restoreBulletSpawnerTile(GameState *state, size_t, size_t, size_t,
                               SerializedTileData *data) {
  return new BulletSpawnerTile<Tile>{
      state->textureManager.getSprite(data->v2i),
      (Tile * (*)(GameState *, size_t, size_t)) data->vp, data->i2, data->i};
}

template <typename Bullet>
inline SerializedTile BulletSpawnerTile<Bullet>::serialize() const {
  SerializedTileData data;
  data.v2i = sprite.getTextureRect().getPosition();
  data.i = spawnDelay;
  data.i2 = spawnDelayMax;
  data.vp = spawner;
  return SerializedTile{restoreBulletSpawnerTile, data};
}
