#include "PlayerTile.h"

PlayerTile::PlayerTile(sf::Sprite sprite, size_t x, size_t y)
    : Tile{sprite}, moveToX{x}, moveToY{y} {}

bool PlayerTile::update(GameState* state, size_t x, size_t y) {
  if (moveToX == x && moveToY == y) {
    // check if need to start moving
    int vx = state->input.right - state->input.left;
    int vy = state->input.down - state->input.up;
    if (vx != 0 && canMove(state, x, y, vx, 0)) {
      moveToX = x + vx;
    } else if (vy != 0 && canMove(state, x, y, 0, vy)) {
      moveToY = y + vy;
    } else {
      return false;
    }
    state->moveDelta = 0;
    startedMoving = state->clock->getElapsedTime();
    moveBoard(state);
  } else {
    // continue moving
    sf::Time t = state->clock->getElapsedTime();
    state->moveDelta = (t - startedMoving).asSeconds() / TOTAL_MOVE_DELTA;
    if (state->moveDelta > 1) {
      state->moveDelta = 1;
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        if (state->board[x][y][i] == this) {
          state->board[moveToX][moveToY].push_back(this);
          state->board[x][y].erase(state->board[x][y].begin() + i);
          return true;
        }
      }
    }
  }
  return false;
}

void PlayerTile::render(GameState* state, size_t x, size_t y) {
  float delta = (1 - cos(state->moveDelta * 3.14159)) / 2;
  float interX = (x * (1 - delta) + moveToX * delta);
  float interY = (y * (1 - delta) + moveToY * delta);
  sprite.setPosition(interX * TILE_SIZE, interY * TILE_SIZE);
  state->window->draw(sprite);
}

int PlayerTile::getZLayer(GameState* state, size_t x, size_t y) const {
  return 1000;
}
