#include <SFML/Graphics.hpp>

#include "game.h"

int main() {
  sf::RenderWindow window{{800, 600}, "CMake SFML Project"};
  window.setFramerateLimit(60);
  sf::Clock clock;
  gameState state;
  loadResources(&state);
  initialize(&state);
  while (window.isOpen()) {
    for (sf::Event event{}; window.pollEvent(event);) {
      handleEvent(&window, &state, &event);
    }
    update(&clock, &state);
    window.clear();
    render(&window, &state);
    window.display();
  }
}
