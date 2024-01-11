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

sf::Sprite TextureManager::getSprite(sf::Vector2i pos) {
  return sf::Sprite{spriteSheet.getTexture(),
                    sf::IntRect{pos, {TILE_SIZE, TILE_SIZE}}};
}

sf::Vector2u posAtIndex(size_t i) {
  // de-interleave, x and y should take the even and odd bits, respectively
  size_t x = i & 0x5555555555555555U;
  size_t y = (i & 0xAAAAAAAAAAAAAAAAU) >> 1;
  x = (x | (x >> 1)) & 0x3333333333333333U;
  x = (x | (x >> 2)) & 0x0F0F0F0F0F0F0F0FU;
  x = (x | (x >> 4)) & 0x00FF00FF00FF00FFU;
  x = (x | (x >> 8)) & 0x0000FFFF0000FFFFU;
  x = (x | (x >> 16)) & 0x00000000FFFFFFFFU;
  y = (y | (y >> 1)) & 0x3333333333333333U;
  y = (y | (y >> 2)) & 0x0F0F0F0F0F0F0F0FU;
  y = (y | (y >> 4)) & 0x00FF00FF00FF00FFU;
  y = (y | (y >> 8)) & 0x0000FFFF0000FFFFU;
  y = (y | (y >> 16)) & 0x00000000FFFFFFFFU;
  return {(unsigned int)x, (unsigned int)y};
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
