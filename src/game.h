#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.h"

///@brief Prepares initial game statte. Called before loadResources()
void initialize(GameState* state, sf::RenderWindow* window, sf::Clock* clock);
void initialize(GameState* state, sf::Window* windowW,
                sf::RenderTarget* windowRT, sf::Clock* clock);

/// @brief Loads all resources needed by the game. Called after initialize() and
/// before setupBoard().
void loadResources(GameState* state);

/// @brief Loads level. Called after loadResources().
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
