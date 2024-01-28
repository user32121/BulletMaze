#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"

/// @brief Loads all resources needed by the game.
void loadResources(GameState* state);

/// @brief Loads level.
void setupBoard(GameState* state,
                std::string level = "resources/levels/level1.json");

/// @brief Called for each event that occurs
void handleEvent(GameState* state, sf::Event* event);

/// @brief Performs game logic.
void update(GameState* state);

/// @brief Performs rendering calls.
void render(GameState* state);

/// @brief Cleans up any resources used.
void uninitialize(GameState* state);
