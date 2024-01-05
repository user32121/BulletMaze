#include "GameState.h"

void moveBoard(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (Tile* tile : state->board[x][y]) {
        tile->prepareMoveTile(state, x, y);
      }
    }
  }
}

bool canMove(GameState* state, size_t x, size_t y, int vx, int vy) {
  size_t x2 = x + vx;
  size_t y2 = y + vy;
  // unsigned, so never negative
  if (x2 >= state->board.size() || y2 >= state->board[0].size()) {
    return false;
  }
  // TODO check for obstacles
  return true;
}
