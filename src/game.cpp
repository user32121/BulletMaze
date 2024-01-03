#include "game.h"

#include <SFML/Graphics.hpp>

void loadResources(gameState* state) {
  state->spriteSheet.loadFromFile("resources/SpriteSheet.png");
  for (size_t i = 0; i < 4; i++) {
    state->floorSprites[i].setTexture(state->spriteSheet);
    state->floorSprites[i].setTextureRect(
        {int(TILE_SIZE * i), 0, TILE_SIZE, TILE_SIZE});
  }
}

void initialize(gameState* state) {
  std::vector<tile> floors;
  for (size_t i = 0; i < 4; ++i) {
    floors.push_back({state->floorSprites[i]});
  }
  state->board.resize(7);
  for (size_t x = 0; x < 7; ++x) {
    state->board[x].resize(5);
    for (size_t y = 0; y < 5; y++) {
      state->board[x][y].push_back(floors[rand() % 4]);
    }
  }
}

void handleEvent(sf::RenderWindow* window, gameState* state, sf::Event* event) {
  if (event->type == sf::Event::Closed) {
    window->close();
  }
}

void update(sf::Clock* clock, gameState* state) {}

void render(sf::RenderWindow* window, gameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (auto&& tile : state->board[x][y]) {
        tile.render(window, x, y);
      }
    }
  }
}
