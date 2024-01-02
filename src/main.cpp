#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window{{800, 600}, "CMake SFML Project"};
  window.setFramerateLimit(60);

  sf::Clock clock;
  while (window.isOpen()) {
    for (sf::Event event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    // window.clear();
    window.display();
  }
}