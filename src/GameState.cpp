#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, sf::Clock* clock)
    : GameState{window, window, clock} {}

GameState::GameState(sf::Window* windowW, sf::RenderTarget* windowRT,
                     sf::Clock* clock)
    : windowW{windowW}, windowRT{windowRT}, clock{clock} {}

void moveBoard(GameState* state) {
  state->history.push_back({});
  state->history.back().resize(state->board.size());
  for (size_t x = 0; x < state->board.size(); ++x) {
    state->history.back()[x].resize(state->board[x].size());
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      state->history.back()[x][y].resize(state->board[x][y].size());
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        state->history.back()[x][y][i] = state->board[x][y][i]->serialize();
      }
    }
  }

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
    int ret = 0;
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
    if (x2 >= state->board.size() || y2 >= state->board[x].size() ||
        y2 >= state->board[x2].size()) {
      return false;
    }
    for (size_t i = 0; i < state->board[x2][y2].size(); ++i) {
      if (state->board[x2][y2][i]->isSolidFor(state, x2, y2, i, curTile)) {
        return false;
      }
    }

    return true;
  }
