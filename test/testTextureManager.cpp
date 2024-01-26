#include "../libs/doctest/doctest.h"
#include "../src/TextureManager.h"

TEST_CASE("sanity check sprite size") {
  TextureManager tm;
  sf::Sprite sprite = tm.getSprite("test_resources/textures/img0.png");
  sf::Vector2i s = sprite.getTextureRect().getSize();
  CHECK(s.x == TILE_SIZE);
  CHECK(s.y == TILE_SIZE);
}

TEST_CASE("ensure enough pixels to hold images (getSprite)") {
  TextureManager tm;
  for (size_t i = 0; i < 20; ++i) {
    sf::Sprite sprite = tm.getSprite("test_resources/textures/img" +
                                     std::to_string(i) + ".png");
    sf::Vector2u s = sprite.getTexture()->getSize();
    CHECK(s.x * s.y >= TILE_SIZE * TILE_SIZE * (i + 1));
  }
}

TEST_CASE("ensure enough pixels to hold images (constructor)") {
  for (size_t i = 1; i < 20; ++i) {
    TextureManager tm{i};
    sf::Sprite sprite = tm.getSprite("test_resources/textures/img0.png");
    sf::Vector2u s = sprite.getTexture()->getSize();
    CHECK(s.x * s.y >= TILE_SIZE * TILE_SIZE * i);
  }
}
