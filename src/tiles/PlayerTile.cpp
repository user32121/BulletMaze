#include "PlayerTile.h"

#include <cmath>

PlayerTile::PlayerTile(sf::Sprite sprite, size_t x, size_t y)
    : Tile{sprite}, moveToX{x}, moveToY{y} {}

void PlayerTile::checkAlive(GameState* state) {
  int value = getBulletValueAt(state, moveToX, moveToY);
  if (value < state->safeRangeMin || value > state->safeRangeMax) {
    // TODO update state
    puts("player died");
    alive = false;
  }
}

void PlayerTile::update(GameState* state, size_t x, size_t y, size_t) {
  if (moveToX == x && moveToY == y) {
    if (state->input.presses.size()) {
      // check if need to start moving
      int vx = 0;
      int vy = 0;
      sf::Keyboard ::Key k = state->input.presses.front();
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
        default:
          // NO OP
          break;
      }
      if (alive && (vx != 0 || vy != 0) &&
          canMove(state, moveToX, moveToY, vx, vy, this)) {
        moveBoard(state);
      } else {
        // consume input
        state->input.presses.pop();
      }
    }
  } else {
    // continue moving
    sf::Time t = state->clock->getElapsedTime();
    state->moveDelta = (t - startedMoving).asSeconds() / TOTAL_MOVE_DELTA;
    if (state->moveDelta > 1 || state->input.presses.size()) {
      state->moveDelta = 1;
      finishMoveBoard(state);
      checkAlive(state);
    }
  }
}

void PlayerTile::render(GameState* state, size_t x, size_t y, size_t) {
  if (!alive) {
    return;
  }
  float delta = float((1 - cos(state->moveDelta * 3.14159)) / 2);
  float interX = (x * (1 - delta) + moveToX * delta);
  float interY = (y * (1 - delta) + moveToY * delta);
  sprite.setPosition(interX * TILE_SIZE, interY * TILE_SIZE);
  state->window->draw(sprite);
}

int PlayerTile::getZLayer(GameState*, size_t, size_t, size_t) const {
  return 1000;
}

void PlayerTile::prepareMove(GameState* state, size_t, size_t, size_t) {
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
    default:
      // NO OP
      break;
  }
  moveToX = moveToX + vx;
  moveToY = moveToY + vy;
  startedMoving = state->clock->getElapsedTime();
}

bool PlayerTile::finishMove(GameState* state, size_t x, size_t y, size_t i) {
  if (moveToX == x && moveToY == y) {
    return false;
  }
  state->board[moveToX][moveToY].push_back(this);
  state->board[x][y].erase(state->board[x][y].begin() + i);
  return true;
}
