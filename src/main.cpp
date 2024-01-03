#include <SFML/Graphics.hpp>

int main()
{
  // TODO get a resource manager
  sf::Texture texture;
  texture.create(64, 64);
  sf::Uint8 pixels[64 * 64 * 4];
  for (size_t i = 0; i < 64 * 64 * 4; ++i) {
    pixels[i] = (i % 4) != 1 ? 0xFF : 0;
  }
  texture.update(pixels);
  texture.loadFromFile("resources/textures/Untitled.png");
  sf::Sprite sprite{texture};

  sf::RenderWindow window{{800, 600}, "CMake SFML Project"};
  window.setFramerateLimit(60);

  sf::Clock clock;
  while (window.isOpen()) {
    for (sf::Event event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.draw(sprite);

    // window.clear();//
    window.display();
  }
}