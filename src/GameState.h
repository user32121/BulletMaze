#pragma once

struct GameState;
#include "tiles/Tile.h"

#define TOTAL_MOVE_DELTA 0.15f

struct Input {
  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
};

struct GameState {
  sf::RenderWindow* window;
  sf::Clock* clock;

  sf::Texture spriteSheet;

  Input input;
  // value in [0,1] for interpolating movement
  float moveDelta = 0;

  std::vector<std::vector<std::vector<Tile*>>> board;
};

/// @brief notifies all tiles to start moving to their next tile
void moveBoard(GameState* state);

/// @brief checks if the position at (x+vx, y+vy) is in bounds and not blocked
bool canMove(GameState* state, size_t x, size_t y, int vx, int vy);
