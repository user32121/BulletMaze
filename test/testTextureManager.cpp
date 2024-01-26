#include "../libs/doctest/doctest.h"
#include "../src/TextureManager.h"

TEST_CASE("sanity check") {
  TextureManager tm;
  auto q = tm.getSprite("q");
  CHECK(q.getTexture()->getSize().x > 0);
}