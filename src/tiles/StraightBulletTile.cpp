#include "StraightBulletTile.h"

StraightBulletTile::StraightBulletTile(sf::Sprite sprite, size_t x, size_t y,
                                       DIRECTION dir)
    : Tile{sprite}, moveToX{x}, moveToY{y}, dir{dir} {}

int StraightBulletTile::getZLayer(GameState* state, size_t x, size_t y) const {
  return 20;
}

// TODO extract movement behavior to a dedicated function
bool StraightBulletTile::update(GameState* state, size_t x, size_t y) {
  if (state->moveDelta >= 1 && (moveToX != x || moveToY != y || collided)) {
    for (size_t i = 0; i < state->board[x][y].size(); ++i) {
      if (state->board[x][y][i] == this) {
        if (!collided) {
          state->board[moveToX][moveToY].push_back(this);
        }
        state->board[x][y].erase(state->board[x][y].begin() + i);
        return true;
      }
    }
    return true;
  }
  return false;
}

void StraightBulletTile::prepareMove(GameState* state, size_t x, size_t y) {
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

void StraightBulletTile::render(GameState* state, size_t x, size_t y) {
  float delta = state->moveDelta;
  float interX = (x * (1 - delta) + moveToX * delta);
  float interY = (y * (1 - delta) + moveToY * delta);
  sprite.setPosition(interX * TILE_SIZE, interY * TILE_SIZE);
  sprite.setColor(
      sf::Color{255, 255, 255, sf::Uint8(collided ? 255 * (1 - delta) : 255)});
  state->window->draw(sprite);
}
