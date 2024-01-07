#include "StraightBulletTile.h"

StraightBulletTile::StraightBulletTile(sf::Sprite sprite, size_t x, size_t y,
                                       DIRECTION dir, int value)
    : Tile{sprite}, dir{dir}, moveToX{x}, moveToY{y}, value{value} {}

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

void StraightBulletTile::render(GameState* state, size_t x, size_t y, size_t) {
  float delta = state->moveDelta;
  float interX = (x * (1 - delta) + moveToX * delta);
  float interY = (y * (1 - delta) + moveToY * delta);
  sprite.setPosition(interX * TILE_SIZE, interY * TILE_SIZE);
  sprite.setColor(
      sf::Color{255, 255, 255, sf::Uint8(collided ? 255 * (1 - delta) : 255)});
  state->window->draw(sprite);
}

// TODO render based on value
int StraightBulletTile::getBulletValue(GameState*, size_t, size_t,
                                       size_t) const {
  return value;
}
