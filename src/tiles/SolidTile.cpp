#include "SolidTile.h"

SolidTile::SolidTile(sf::Sprite sprite) : Tile{sprite} {}

SolidTile::SolidTile(GameState *state, nlohmann::json *json, size_t, size_t,
                     size_t)
    : SolidTile{state->textureManager.getSprite(json->value("sprite", ""))} {}

bool SolidTile::isSolidFor(GameState*, size_t, size_t, size_t, Tile*) const {
  return true;
}

int SolidTile::getZLayer(GameState*, size_t, size_t, size_t) const { return 5; }

static Tile *restore(GameState *state, size_t, size_t, size_t,
                     SerializedTileData *data) {
  return new SolidTile{state->textureManager.getSprite(data->v2i)};
}

SerializedTile SolidTile::serialize() const {
  SerializedTileData data;
  data.v2i = sprite.getTextureRect().getPosition();
  return SerializedTile{restore, data};
}
