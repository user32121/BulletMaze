#include "TextureManager.h"

void TextureManager::increaseSize() {
  sf::Vector2u spriteSheetSize = spriteSheet.getSize();
  sf::RenderTexture temp;
  temp.create(spriteSheetSize.x, spriteSheetSize.y);
  temp.draw(sf::Sprite{spriteSheet.getTexture()}, sf::BlendNone);
  temp.display();
  spriteSheet.create(spriteSheetSize.x * 2, spriteSheetSize.y * 2);
  spriteSheet.draw(sf::Sprite{temp.getTexture()}, sf::BlendNone);
}

TextureManager::TextureManager(size_t capacity) {
  // find min size that will hold capacity
  unsigned int n = 0;
  --capacity;
  while (capacity) {
    capacity >>= 2;
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
                    sf::IntRect(pos.x, pos.y, TILE_SIZE, TILE_SIZE)};
}

sf::Vector2u posAtIndex(size_t i) {
  // TODO O(1) (ASM) de-interleave
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int bit = 1;
  while (i) {
    x |= i & bit;
    y |= (i & (bit << 1)) >> 1;
    bit <<= 1;
    i >>= 1;
  }
  return {x, y};
}

void TextureManager::loadTexture(const std::string& filename) {
  sf::Vector2u spriteSheetSize = spriteSheet.getSize();
  if (size * TILE_SIZE * TILE_SIZE >= spriteSheetSize.x * spriteSheetSize.y) {
    increaseSize();
  }
  sf::Texture texture;
  texture.loadFromFile(filename);
  sf::Sprite sprite{texture};
  sf::Vector2u pos = posAtIndex(size) * TILE_SIZE;
  sprite.setPosition((float)pos.x, (float)pos.y);
  spriteSheet.draw(sprite, sf::BlendNone);
  spriteSheet.display();
  locations[filename] = pos;
  ++size;
}
