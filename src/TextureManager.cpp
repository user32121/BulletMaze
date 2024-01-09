#include "TextureManager.h"

void TextureManager::increaseSize() {
  sf::Vector2u spriteSheetSize = spriteSheet.getSize();
  spriteSheet.create(spriteSheetSize.x * 2, spriteSheetSize.y * 2);
}

TextureManager::TextureManager(size_t capacity = 1) {
  // find min size that will hold capacity
  unsigned int n = 0;
  --capacity;
  while (capacity) {
    capacity <<= 2;
    ++n;
  }

  spriteSheet.create(TILE_SIZE * (1 << n), TILE_SIZE * (1 << n));
}

sf::Sprite TextureManager::getSprite(const std::string& filename) {
  if (!locations.count(filename)) {
    loadTexture(filename);
  }
  sf::Vector2u pos = locations[filename];
  return sf::Sprite{spriteSheet.getTexture(),
                    sf::IntRect{pos.x, pos.y, TILE_SIZE, TILE_SIZE}};
}

sf::Vector2u posAtIndex(size_t i) {
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int bit = 0;
  while (i) {
    x |= i & bit;
    y |= i & (bit << 1);
    bit <<= 1;
    i >>= 2;
  }
}

void TextureManager::loadTexture(const std::string& filename) {
  if (locations.count(filename)) {
    return;
  }
  sf::Vector2u spriteSheetSize = spriteSheet.getSize();
  if (size >= spriteSheetSize.x * spriteSheetSize.y) {
    increaseSize();
  }
  sf::Texture texture;
  texture.loadFromFile(filename);
  sf::Sprite sprite{texture};
  sf::Vector2u pos = posAtIndex(size) * TILE_SIZE;
  sprite.setPosition(pos.x, pos.y);
  spriteSheet.draw(sprite);
  spriteSheet.display();
  locations[filename] = pos;
  ++size;
}
