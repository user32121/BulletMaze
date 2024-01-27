#include <typeinfo>

#include "../libs/doctest/doctest.h"
#include "../src/GameState.h"
#include "../src/tiles/Tile.h"

void initialize(GameState* state) {
  state->board.resize(1);
  state->board[0].resize(1);
}
void uninitialize(GameState* state) {
  for (size_t i = 0; i < state->board[0][0].size(); ++i) {
    delete state->board[0][0][i];
  }
  state->board.clear();
}

void checkRenders(GameState* state, Tile* t1, Tile* t2) {
  sf::RenderTexture rt1, rt2;
  rt1.create(TILE_SIZE, TILE_SIZE);
  rt2.create(TILE_SIZE, TILE_SIZE);
  state->windowRT = &rt1;
  t1->render(state, 0, 0, 0);
  rt1.display();
  state->windowRT = &rt2;
  t2->render(state, 0, 0, 0);
  rt2.display();

  sf::Image img1 = rt1.getTexture().copyToImage();
  sf::Image img2 = rt2.getTexture().copyToImage();
  for (unsigned int x = 0; x < TILE_SIZE; x++) {
    for (unsigned int y = 0; y < TILE_SIZE; y++) {
      CHECK(img1.getPixel(x, y) == img2.getPixel(x, y));
    }
  }
}

void checkTiles(GameState* state, Tile* t1, Tile* t2) {
  CHECK(typeid(*t1) == typeid(*t2));
  checkRenders(state, t1, t2);
  CHECK(t1->getZLayer(state, 0, 0, 0) == t2->getZLayer(state, 0, 0, 0));
  CHECK(t1->getBulletValue(state, 0, 0, 0) ==
        t2->getBulletValue(state, 0, 0, 0));
}

TEST_CASE("serialize Tile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new Tile{
      state.textureManager.getSprite("test_resources/textures/img0.png")};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);
}
