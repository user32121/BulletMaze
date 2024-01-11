#include "BulletSpawnerTile.h"

BulletSpawnerTile::BulletSpawnerTile(sf::Sprite sprite,
                                     Tile *(*spawn)(GameState *state, size_t x,
                                                    size_t y),
                                     int spawnPeriod, int initialSpawnDelay)
    : Tile{sprite},
      spawnDelayMax{spawnPeriod},
      spawnDelay{initialSpawnDelay + 1},
      spawner{spawn} {}

int BulletSpawnerTile::getZLayer(GameState *, size_t, size_t, size_t) const {
  return 10;
}

void BulletSpawnerTile::prepareMove(GameState *state, size_t x, size_t y,
                                    size_t) {
  --spawnDelay;
  if (spawnDelay <= 0) {
    spawnDelay = spawnDelayMax;
    state->board[x][y].push_back(spawner(state, x, y));
  }
}

Tile *restoreBulletSpawnerTile(GameState *state, size_t, size_t, size_t,
                               SerializedTileData *data) {
  return new BulletSpawnerTile{state->textureManager.getSprite(data->v2i),
                               data->fp, data->i2, data->i};
}

SerializedTile BulletSpawnerTile::serialize() const {
  SerializedTileData data;
  data.v2i = sprite.getTextureRect().getPosition();
  data.i = spawnDelay;
  data.i2 = spawnDelayMax;
  data.fp = spawner;
  return SerializedTile{restoreBulletSpawnerTile, data};
}
