#include <SFML/Graphics.hpp>

#include "game.h"

int main() {
  sf::RenderWindow window{{800, 600}, "Bullet Maze"};
  window.setFramerateLimit(60);
  sf::Clock clock;
  GameState state;
  state.windowW = &window;
  state.windowRT = &window;
  state.clock = &clock;

  loadResources(&state);
  initialize(&state);
  while (window.isOpen()) {
    for (sf::Event event{}; window.pollEvent(event);) {
      handleEvent(&state, &event);
    }
    update(&state);
    window.clear();
    render(&state);
    window.display();
  }
  uninitialize(&state);
}
