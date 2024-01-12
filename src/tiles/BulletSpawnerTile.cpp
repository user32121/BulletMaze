#include "BulletSpawnerTile.h"

#include "TileUtils.h"

BulletSpawnerTile::BulletSpawnerTile(
    sf::Sprite sprite, Tile *(*spawn)(GameState *state, size_t x, size_t y),
    int spawnPeriod, int initialSpawnDelay, nlohmann::json altSpawnData)
    : Tile{sprite},
      spawnDelayMax{spawnPeriod},
      spawnDelay{initialSpawnDelay + 1},
      spawner{spawn},
      altSpawnData(altSpawnData) {}

BulletSpawnerTile::BulletSpawnerTile(GameState *state, nlohmann::json *json,
                                     size_t, size_t, size_t)
    : BulletSpawnerTile{
          state->textureManager.getSprite(json->value("sprite", "")), nullptr,
          json->value("spawnPeriod", 0), json->value("initialSpawnDelay", 0),
          (*json)["bullet"]} {}

int BulletSpawnerTile::getZLayer(GameState *, size_t, size_t, size_t) const {
  return 10;
}

void BulletSpawnerTile::prepareMove(GameState *state, size_t x, size_t y,
                                    size_t i) {
  --spawnDelay;
  if (spawnDelay <= 0) {
    spawnDelay = spawnDelayMax;
    Tile *tile;
    if (spawner == nullptr) {
      try {
        tile = jsonToTile(state, &altSpawnData, x, y, i);
      } catch (const std::exception &e) {
        printf("An exception ocurred while spawning bullet:\n  %s\n", e.what());
        return;
      }
    } else {
      tile = (spawner(state, x, y));
    }
    state->board[x][y].push_back(tile);
  }
}

Tile *restoreBulletSpawnerTile(GameState *state, size_t, size_t, size_t,
                               SerializedTileData *data) {
  return new BulletSpawnerTile{state->textureManager.getSprite(data->v2i),
                               data->fp, data->i2, data->i, data->json};
}

SerializedTile BulletSpawnerTile::serialize() const {
  SerializedTileData data;
  data.v2i = sprite.getTextureRect().getPosition();
  data.i = spawnDelay;
  data.i2 = spawnDelayMax;
  data.fp = spawner;
  data.json = altSpawnData;
  return SerializedTile{restoreBulletSpawnerTile, data};
}
