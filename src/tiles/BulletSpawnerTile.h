#pragma once

#include "../../libs/nlohmann/json.hpp"
#include "Tile.h"

/// @brief Spawns a Bullet periodically
class BulletSpawnerTile : public Tile {
 private:
  // should be a positive value, otherwise funky stuff happens with undo
  unsigned int spawnDelayMax;
  unsigned int spawnDelay;
  Tile *(*spawner)(GameState *state, size_t x, size_t y);
  nlohmann::json altSpawnData;

 public:
  BulletSpawnerTile(sf::Sprite sprite,
                    Tile *(*spawn)(GameState *state, size_t x, size_t y),
                    unsigned int spawnPeriod,
                    unsigned int initialSpawnDelay = 0,
                    nlohmann::json altSpawnData = {});
  BulletSpawnerTile(GameState *state, nlohmann::json *json, size_t x, size_t y,
                    size_t i);

  int getZLayer(GameState *state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState *state, size_t x, size_t y, size_t i) override;
  SerializedTile serialize() const override;
};
