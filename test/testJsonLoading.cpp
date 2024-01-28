#include "../libs/doctest/doctest.h"
#include "../src/GameState.h"
#include "../src/game.h"
#include "../src/tiles/BulletSpawnerTile.h"
#include "../src/tiles/PlayerTile.h"
#include "../src/tiles/SolidTile.h"
#include "../src/tiles/StraightBulletTile.h"

TEST_CASE("load level0.json") {
  GameState state{nullptr, nullptr};
  setupBoard(&state, "test_resources/levels/level0.json");

  // board size
  REQUIRE(state.board.size() == 2);
  REQUIRE(state.board[0].size() == 3);
  REQUIRE(state.board[1].size() == 3);
  REQUIRE(state.board[0][0].size() == 2);
  REQUIRE(state.board[0][1].size() == 2);
  REQUIRE(state.board[0][2].size() == 2);
  REQUIRE(state.board[1][0].size() == 1);
  REQUIRE(state.board[1][1].size() == 2);
  REQUIRE(state.board[1][2].size() == 2);

  // tile types
  CHECK(typeid(*state.board[0][0][0]) == typeid(Tile));
  CHECK(typeid(*state.board[0][0][1]) == typeid(StraightBulletTile));
  CHECK(typeid(*state.board[0][1][0]) == typeid(Tile));
  CHECK(typeid(*state.board[0][1][1]) == typeid(PlayerTile));
  PlayerTile *pt = (PlayerTile *)state.board[0][1][1];
  CHECK(typeid(*state.board[0][2][0]) == typeid(Tile));
  CHECK(typeid(*state.board[0][2][1]) == typeid(SolidTile));
  CHECK(typeid(*state.board[1][0][0]) == typeid(Tile));
  CHECK(typeid(*state.board[1][1][0]) == typeid(Tile));
  CHECK(typeid(*state.board[1][1][1]) == typeid(BulletSpawnerTile));
  CHECK(typeid(*state.board[1][2][0]) == typeid(Tile));
  CHECK(typeid(*state.board[1][2][1]) == typeid(BulletSpawnerTile));

  // tile properties
  CHECK(state.board[0][0][0]->getBulletValue(&state, 0, 0, 0) == 0);
  CHECK(state.board[0][0][0]->isSolidFor(&state, 0, 0, 0, pt) == false);
  CHECK(state.board[0][0][1]->getBulletValue(&state, 0, 0, 1) == 2);
  CHECK(state.board[0][0][1]->isSolidFor(&state, 0, 0, 1, pt) == false);
  CHECK(state.board[0][1][0]->getBulletValue(&state, 0, 1, 0) == 0);
  CHECK(state.board[0][1][0]->isSolidFor(&state, 0, 1, 0, pt) == false);
  CHECK(state.board[0][1][1]->getBulletValue(&state, 0, 1, 1) == 0);
  CHECK(state.board[0][1][1]->isSolidFor(&state, 0, 1, 1, pt) == false);
  CHECK(state.board[0][2][0]->getBulletValue(&state, 0, 2, 0) == 0);
  CHECK(state.board[0][2][0]->isSolidFor(&state, 0, 2, 0, pt) == false);
  CHECK(state.board[0][2][1]->getBulletValue(&state, 0, 2, 1) == 0);
  CHECK(state.board[0][2][1]->isSolidFor(&state, 0, 2, 1, pt) == true);
  CHECK(state.board[1][0][0]->getBulletValue(&state, 1, 0, 0) == 0);
  CHECK(state.board[1][0][0]->isSolidFor(&state, 1, 0, 0, pt) == false);
  CHECK(state.board[1][1][0]->getBulletValue(&state, 1, 1, 0) == 0);
  CHECK(state.board[1][1][0]->isSolidFor(&state, 1, 1, 0, pt) == false);
  CHECK(state.board[1][1][1]->getBulletValue(&state, 1, 1, 1) == 0);
  CHECK(state.board[1][1][1]->isSolidFor(&state, 1, 1, 1, pt) == false);
  CHECK(state.board[1][2][0]->getBulletValue(&state, 1, 2, 0) == 0);
  CHECK(state.board[1][2][0]->isSolidFor(&state, 1, 2, 0, pt) == false);
  CHECK(state.board[1][2][1]->getBulletValue(&state, 1, 2, 1) == 0);
  CHECK(state.board[1][2][1]->isSolidFor(&state, 1, 2, 1, pt) == false);

  state.board[1][1][1]->prepareMove(&state, 1, 1, 1);
  state.board[1][1][1]->finishMove(&state, 1, 1, 1);
  REQUIRE(state.board[1][1].size() == 3);
  CHECK(typeid(*state.board[1][1][2]) == typeid(StraightBulletTile));
  CHECK(state.board[1][1][2]->getBulletValue(&state, 1, 1, 2) == 2);
  CHECK(state.board[1][1][2]->isSolidFor(&state, 1, 1, 2, pt) == false);
  state.board[1][2][1]->prepareMove(&state, 1, 2, 1);
  state.board[1][2][1]->finishMove(&state, 1, 2, 1);
  REQUIRE(state.board[1][2].size() == 3);
  CHECK(typeid(*state.board[1][2][2]) == typeid(StraightBulletTile));
  CHECK(state.board[1][2][2]->getBulletValue(&state, 1, 2, 2) == -1);
  CHECK(state.board[1][2][2]->isSolidFor(&state, 1, 2, 2, pt) == false);
}
