#pragma once

#include <SFML/Graphics.hpp>

struct GameState;
#include "tiles/Tile.h"

struct GameState {
  sf::RenderWindow* window;
  sf::Clock* clock;

  sf::Texture spriteSheet;
  sf::Sprite floorSprites[4];
  std::vector<std::vector<std::vector<Tile>>> board;
};

/// @brief Loads all resources needed by the game. Called before initialize().
void loadResources(GameState* state);
/// @brief Prepares initial game state. Called after loadResources().
void initialize(GameState* state);
/// @brief Called for each event that occurs
void handleEvent(GameState* state, sf::Event* event);
/// @brief Performs game logic.
void update(GameState* state);
/// @brief Performs rendering calls.
void render(GameState* state);
