#include <SFML/Graphics.hpp>

#include "tiles/tile.h"

struct gameState {
  sf::Texture spriteSheet;
  sf::Sprite floorSprites[4];
  std::vector<std::vector<std::vector<tile>>> board;
};

void loadResources(gameState* state);
void initialize(gameState* state);
void handleEvent(sf::RenderWindow* window, gameState* state, sf::Event* event);
void update(sf::Clock* clock, gameState* state);
void render(sf::RenderWindow* window, gameState* state);
