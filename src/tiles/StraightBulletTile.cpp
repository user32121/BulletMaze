#include "StraightBulletTile.h"

#include "../TextureManager.h"

StraightBulletTile::StraightBulletTile(sf::Sprite sprite, size_t x, size_t y,
                                       DIRECTION dir, int value)
    : Tile{sprite}, dir{dir}, moveToX{x}, moveToY{y}, value{value} {}

StraightBulletTile::StraightBulletTile(GameState* state, nlohmann::json* json,
                                       size_t x, size_t y, size_t)
    : StraightBulletTile{
          state->textureManager.getSprite(json->value("sprite", "")), x, y,
          json->value("dir", LEFT), json->value("value", 0)} {}

int StraightBulletTile::getZLayer(GameState*, size_t, size_t, size_t) const {
  return 200;
}

bool StraightBulletTile::finishMove(GameState* state, size_t x, size_t y,
                                    size_t i) {
  if (moveToX == x && moveToY == y && !collided) {
    return false;
  }
  state->board[x][y].erase(state->board[x][y].begin() + i);
  if (collided) {
    delete this;
  } else {
    state->board[moveToX][moveToY].push_back(this);
  }
  return true;
}

void StraightBulletTile::prepareMove(GameState* state, size_t, size_t, size_t) {
  int vx = 0;
  int vy = 0;
  switch (dir) {
    case LEFT:
      vx = -1;
      break;
    case RIGHT:
      vx = 1;
      break;
    case UP:
      vy = -1;
      break;
    case DOWN:
      vy = 1;
      break;
  }
  if (canMove(state, moveToX, moveToY, vx, vy, this)) {
    moveToX += vx;
    moveToY += vy;
  } else {
    collided = true;
  }
}

void StraightBulletTile::render(GameState* state, size_t x, size_t y,
                                size_t i) {
  float delta = state->moveDelta;
  float interX = (x * (1 - delta) + moveToX * delta);
  float interY = (y * (1 - delta) + moveToY * delta);
  sprite.setPosition(interX * TILE_SIZE, interY * TILE_SIZE);
  sprite.setColor(
      sf::Color{255, 255, 255, sf::Uint8(collided ? 255 * (1 - delta) : 255)});

  state->bulletsShader1.setUniform("value", getBulletValue(state, x, y, i));
  state->bulletsRenderTexture.display();
  state->bulletsRenderTexture.draw(
      sprite,
      sf::RenderStates{sf::BlendNone, {}, nullptr, &state->bulletsShader1});
}

int StraightBulletTile::getBulletValue(GameState*, size_t, size_t,
                                       size_t) const {
  return value;
}

static Tile* restore(GameState* state, size_t x, size_t y, size_t,
                     SerializedTileData* data) {
  return new StraightBulletTile{state->textureManager.getSprite(data->v2i), x,
                                y, (DIRECTION)data->i2, data->i};
}

SerializedTile StraightBulletTile::serialize() const {
  SerializedTileData data;
  data.v2i = sprite.getTextureRect().getPosition();
  data.i = value;
  data.i2 = dir;
  return SerializedTile{restore, data};
}
