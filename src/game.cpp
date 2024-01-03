#include "game.h"

#include <SFML/Graphics.hpp>

void loadResources(gameState* state) {
  state->texture.loadFromFile("resources/textures/Untitled.png");
  state->sprite.setTexture(state->texture);
}

void handleEvent(sf::RenderWindow* window, gameState* state, sf::Event* event) {
  if (event->type == sf::Event::Closed) {
    window->close();
  }
}

void update(sf::Clock* clock, gameState* state) {
  float t = clock->getElapsedTime().asSeconds();
  state->sprite.setPosition({100 + 100 * sin(2 * t), 100 + 100 * sin(3 * t)});
}

void render(sf::RenderWindow* window, gameState* state) {
  window->draw(state->sprite);
}
