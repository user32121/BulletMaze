#pragma once

#include <functional>

#include "Tile.h"

/// @brief Spawns a Bullet periodically
class BulletSpawnerTile : public Tile {
 private:
  int spawnDelayMax;
  int spawnDelay;
  Tile *(*spawner)(GameState *state, size_t x, size_t y);

 public:
  BulletSpawnerTile(sf::Sprite sprite,
                    Tile *(*spawn)(GameState *state, size_t x, size_t y),
                    int spawnPeriod, int initialSpawnDelay = 0);

  int getZLayer(GameState *state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState *state, size_t x, size_t y, size_t i) override;
  SerializedTile serialize() const override;
};
