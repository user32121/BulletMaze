#pragma once

#include <queue>

#include "TextureManager.h"
#include "tiles/Tile.h"

#define TOTAL_MOVE_DELTA 0.15f

struct Input {
  std::queue<sf::Keyboard::Key> presses;
};

class Tile;
struct SerializedTile;
struct GameState {
  GameState(sf::RenderWindow* window, sf::Clock* clock);
  GameState(sf::Window* windowW, sf::RenderTarget* windowRT, sf::Clock* clock);

  GameState(const GameState& other) = delete;
  GameState& operator=(const GameState& other) = delete;
  ~GameState();

  // pointers for program interaction
  sf::Window* windowW = nullptr;
  sf::RenderTarget* windowRT = nullptr;
  sf::Clock* clock = nullptr;

  // resources
  TextureManager textureManager;
  sf::Shader bulletsShader1;
  sf::Shader bulletsShader2;
  sf::RenderTexture bulletsRenderTexture;
  sf::Sprite bulletsSprite;

  // input and movement
  Input input;
  // value in [0,1] for interpolating movement
  float moveDelta = 0;

  // game data
  std::string curLevel;
  std::vector<std::vector<std::vector<Tile*>>> board;
  int safeRangeMin = 0;
  int safeRangeMax = 0;
  // 4D vector wooo [time, x, y, i]
  std::vector<std::vector<std::vector<std::vector<SerializedTile>>>> history;
};

/// @brief checks if the position at (x+vx, y+vy) is in bounds and not blocked
bool canMove(GameState* state, size_t x, size_t y, int vx, int vy,
             Tile* curTile);

/// @brief notifies all tiles to start moving to their next tile
void moveBoard(GameState* state);

/// @brief notifies all tiles to finish their move action
void finishMoveBoard(GameState* state);

/// @brief The sum of tile bullet values at a postiion
int getBulletValueAt(GameState* state, size_t x, size_t y);
