#include "PlayerTile.h"

#include <cmath>

PlayerTile::PlayerTile(sf::Sprite sprite, size_t x, size_t y)
    : Tile{sprite}, moveToX{x}, moveToY{y} {}

bool PlayerTile::update(GameState* state, size_t x, size_t y) {
  if (moveToX == x && moveToY == y) {
    if (state->input.presses.size()) {
      // check if need to start moving
      int vx = 0;
      int vy = 0;
      sf::Keyboard::Key k = state->input.presses.front();
      state->input.presses.pop();
      switch (k) {
        case sf::Keyboard::Left:
          vx = -1;
          break;
        case sf::Keyboard::Right:
          vx = 1;
          break;
        case sf::Keyboard::Up:
          vy = -1;
          break;
        case sf::Keyboard::Down:
          vy = 1;
          break;
      }
      if (vx != 0 && canMove(state, moveToX, moveToY, vx, 0, this)) {
        moveToX = moveToX + vx;
      } else if (vy != 0 && canMove(state, moveToX, moveToY, 0, vy, this)) {
        moveToY = moveToY + vy;
      } else {
        return false;
      }
      state->moveDelta = 0;
      startedMoving = state->clock->getElapsedTime();
      moveBoard(state);
    }
  } else {
    // continue moving
    sf::Time t = state->clock->getElapsedTime();
    state->moveDelta = (t - startedMoving).asSeconds() / TOTAL_MOVE_DELTA;
    if (state->moveDelta > 1 || state->input.presses.size()) {
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
