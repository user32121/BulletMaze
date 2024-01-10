#include "Tile.h"

#include "../TextureManager.h"

Tile::Tile(sf::Sprite sprite) : sprite{sprite} {}

bool Tile::finishMove(GameState*, size_t, size_t, size_t) { return false; }

void Tile::render(GameState* state, size_t x, size_t y, size_t) {
  sprite.setPosition(float(x * TILE_SIZE), float(y * TILE_SIZE));
  state->window->draw(sprite);
}

int Tile::getZLayer(GameState*, size_t, size_t, size_t) const { return 0; }

bool Tile::isSolidFor(GameState*, size_t, size_t, size_t, Tile*) const {
  return false;
}

int Tile::getBulletValue(GameState*, size_t, size_t, size_t) const { return 0; }

SerializedTile Tile::serialize() const {
  return SerializedTile{restore, sprite.getTextureRect().getPosition()};
}

Tile *Tile::restore(GameState *state, size_t, size_t, size_t,
                    SerializedTileData *data) {
  return new Tile{state->textureManager.getSprite(data->texturePos)};
}

void Tile::update(GameState*, size_t, size_t, size_t) {}

void Tile::prepareMove(GameState*, size_t, size_t, size_t) {}
