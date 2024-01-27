#include <typeinfo>

#include "../libs/doctest/doctest.h"
#include "../src/GameState.h"
#include "../src/tiles/BulletSpawnerTile.h"
#include "../src/tiles/PlayerTile.h"
#include "../src/tiles/SolidTile.h"
#include "../src/tiles/StraightBulletTile.h"
#include "../src/tiles/Tile.h"

void initialize(GameState* state) {
  state->board.resize(1);
  state->board[0].resize(1);
  state->bulletsRenderTexture.create(TILE_SIZE, TILE_SIZE);
}
void uninitialize(GameState* state) {
  for (size_t i = 0; i < state->board[0][0].size(); ++i) {
    delete state->board[0][0][i];
  }
  state->board.clear();
}

void checkRenders(GameState* state, Tile* t1, Tile* t2) {
  sf::RenderTexture rt;
  rt.create(TILE_SIZE, TILE_SIZE);
  state->windowRT = &rt;

  rt.clear();
  state->bulletsRenderTexture.clear();
  t1->render(state, 0, 0, 0);
  rt.display();
  state->bulletsRenderTexture.display();
  sf::Image screen1 = rt.getTexture().copyToImage();
  sf::Image bullets1 = state->bulletsRenderTexture.getTexture().copyToImage();
  rt.clear();
  state->bulletsRenderTexture.clear();
  t2->render(state, 0, 0, 0);
  rt.display();
  state->bulletsRenderTexture.display();
  sf::Image screen2 = rt.getTexture().copyToImage();
  sf::Image bullets2 = state->bulletsRenderTexture.getTexture().copyToImage();

  for (unsigned int x = 0; x < TILE_SIZE; x++) {
    for (unsigned int y = 0; y < TILE_SIZE; y++) {
      CHECK(screen1.getPixel(x, y) == screen2.getPixel(x, y));
      CHECK(bullets1.getPixel(x, y) == bullets2.getPixel(x, y));
    }
  }
}

void checkTiles(GameState* state, Tile* t1, Tile* t2) {
  CHECK(typeid(*t1) == typeid(*t2));
  checkRenders(state, t1, t2);
  CHECK(t1->getZLayer(state, 0, 0, 0) == t2->getZLayer(state, 0, 0, 0));
  CHECK(t1->getBulletValue(state, 0, 0, 0) ==
        t2->getBulletValue(state, 0, 0, 0));
  sf::Sprite s =
      state->textureManager.getSprite("test_resources/textures/img0.png");
  PlayerTile* pt = new PlayerTile{s, s, 0, 0};
  CHECK(t1->isSolidFor(state, 0, 0, 0, pt) ==
        t2->isSolidFor(state, 0, 0, 0, pt));
  delete pt;
}

TEST_CASE("serialize Tile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new Tile{
      state.textureManager.getSprite("test_resources/textures/img0.png")};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);

  delete t1;
  delete t2;
  uninitialize(&state);
}

TEST_CASE("serialize SolidTile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new SolidTile{
      state.textureManager.getSprite("test_resources/textures/img0.png")};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);

  delete t1;
  delete t2;
  uninitialize(&state);
}

TEST_CASE("serialize PlayerTile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new PlayerTile{
      state.textureManager.getSprite("test_resources/textures/img0.png"),
      state.textureManager.getSprite("test_resources/textures/img1.png"), 0, 0};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);

  delete t1;
  delete t2;
  uninitialize(&state);
}

TEST_CASE("serialize StraightBulletTile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new StraightBulletTile{
      state.textureManager.getSprite("test_resources/textures/img0.png"), 0, 0,
      RIGHT};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);

  delete t1;
  delete t2;
  uninitialize(&state);
}

TEST_CASE("serialize BulletSpawnerTile") {
  GameState state;
  initialize(&state);

  Tile* t1 = new BulletSpawnerTile{
      state.textureManager.getSprite("test_resources/textures/img0.png"),
      [](GameState* state, size_t, size_t) {
        return new Tile{state->textureManager.getSprite(
            "test_resources/textures/img0.png")};
      },
      0, 0, 0};
  SerializedTile st = t1->serialize();
  Tile* t2 = st.restore(&state, 0, 0, 0, &st.data);

  checkTiles(&state, t1, t2);

  // check spawns
  state.board[0][0].push_back(t1);
  t1->prepareMove(&state, 0, 0, 0);
  t1->finishMove(&state, 0, 0, 0);
  REQUIRE(state.board[0][0].size() == 2);
  Tile* spawn1 = state.board[0][0][1];
  state.board[0][0].clear();
  state.board[0][0].push_back(t2);
  t2->prepareMove(&state, 0, 0, 0);
  t2->finishMove(&state, 0, 0, 0);
  REQUIRE(state.board[0][0].size() == 2);
  Tile* spawn2 = state.board[0][0][1];
  state.board[0][0].clear();
  checkTiles(&state, t1, t2);
  checkTiles(&state, spawn1, spawn2);

  delete t1;
  delete t2;
  delete spawn1;
  delete spawn2;
  uninitialize(&state);
}
