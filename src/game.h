#include <SFML/Graphics.hpp>

struct gameState {
  sf::Texture texture;
  sf::Sprite sprite;
};

void loadResources(gameState* state);
void handleEvent(sf::RenderWindow* window, gameState* state, sf::Event* event);
void update(sf::Clock* clock, gameState* state);
void render(sf::RenderWindow* window, gameState* state);
