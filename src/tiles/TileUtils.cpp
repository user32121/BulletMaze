#include "TileUtils.h"

#include "BulletSpawnerTile.h"
#include "PlayerTile.h"
#include "SolidTile.h"
#include "StraightBulletTile.h"

Tile* jsonToTile(GameState* state, nlohmann::json* json, size_t x, size_t y,
                 size_t i) {
  std::string tileType = json->value("type", "");
  if (tileType == "Tile") {
    return new Tile{state, json, x, y, i};
  } else if (tileType == "PlayerTile") {
    return new PlayerTile{state, json, x, y, i};
  } else if (tileType == "SolidTile") {
    return new SolidTile{state, json, x, y, i};
  } else if (tileType == "StraightBulletTile") {
    return new StraightBulletTile{state, json, x, y, i};
  } else if (tileType == "BulletSpawnerTile") {
    return new BulletSpawnerTile{state, json, x, y, i};
  } else {
    throw std::invalid_argument{
        ("Unhandled tile type: \"" + tileType + "\"").c_str()};
  }
}
