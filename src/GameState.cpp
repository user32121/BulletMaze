#include "GameState.h"

void moveBoard(GameState* state) {
  state->moveDelta = 0;
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        state->board[x][y][i]->prepareMove(state, x, y, i);
      }
    }
  }
}

void finishMoveBoard(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        if (state->board[x][y][i]->finishMove(state, x, y, i)) {
          --i;
        }
      }
    }
  }
}

int getBulletValueAt(GameState* state, size_t x, size_t y) {
  int ret;
  for (size_t i = 0; i < state->board[x][y].size(); ++i) {
    ret += state->board[x][y][i]->getBulletValue(state, x, y, i);
  }
  return ret;
}

bool canMove(GameState* state, size_t x, size_t y, int vx, int vy,
             Tile* curTile) {
  size_t x2 = x + vx;
  size_t y2 = y + vy;
  // unsigned, so never negative
  if (x2 >= state->board.size() || y2 >= state->board[0].size()) {
    return false;
  }
  for (size_t i = 0; i < state->board[x2][y2].size(); ++i) {
    if (state->board[x2][y2][i]->isSolidFor(state, x2, y2, i, curTile)) {
      return false;
    }
  }

  return true;
}
