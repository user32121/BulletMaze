#include "game.h"

#include <SFML/Graphics.hpp>
#include <exception>
#include <fstream>

#define JSON_ImplicitConversions 0
#include "../libs/nlohmann/json.hpp"
#include "TextureManager.h"
#include "tiles/BulletSpawnerTile.h"
#include "tiles/PlayerTile.h"
#include "tiles/SolidTile.h"
#include "tiles/StraightBulletTile.h"

void loadResources(GameState* state) {
  if (!sf::Shader::isAvailable()) {
    puts("Shaders are not available on this system");
  }
  state->bulletsShader1.loadFromFile("resources/shaders/bullets1.vert",
                                     "resources/shaders/bullets1.frag");
  state->bulletsShader2.loadFromFile("resources/shaders/bullets2.frag",
                                     sf::Shader::Fragment);
  state->bulletsShader1.setUniform("texture", sf::Shader::CurrentTexture);
  state->bulletsShader2.setUniform("texture", sf::Shader::CurrentTexture);
}

void loadFallbackLevel(GameState* state) {
  constexpr size_t width = 5;
  constexpr size_t height = 5;
  srand(1);

  // floor
  static std::string floorTextures[4] = {
      "resources/textures/floor0.png",
      "resources/textures/floor1.png",
      "resources/textures/floor2.png",
      "resources/textures/floor3.png",
  };
  state->board.resize(width);
  for (size_t x = 0; x < width; ++x) {
    state->board[x].resize(height);
    for (size_t y = 0; y < height; y++) {
      state->board[x][y].push_back(
          new Tile{state->textureManager.getSprite(floorTextures[rand() % 4])});
    }
  }
  // player
  state->board[0][0].push_back(new PlayerTile{
      state->textureManager.getSprite("resources/textures/player.png"),
      state->textureManager.getSprite("resources/textures/playerDead.png"), 0,
      0});

  // random obstacles
  for (size_t i = 0; i < 3; ++i) {
    state->board[rand() % width][rand() % height].push_back(new SolidTile{
        state->textureManager.getSprite("resources/textures/obstacle.png")});
  }
}

Tile* jsonToTile(GameState* state, nlohmann::json* json, size_t x, size_t y,
                 size_t i) {
  std::string tileType = json->value("type", "");
  if (tileType == "Tile") {
    return new Tile(state, json, x, y, i);
  } else if (tileType == "PlayerTile") {
    return new PlayerTile(state, json, x, y, i);
  } else {
    throw std::invalid_argument{
        ("Unhandled tile type: \"" + tileType + "\"").c_str()};
  }
}

void clearBoard(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        delete state->board[x][y][i];
      }
      state->board[x][y].clear();
    }
  }
}

void loadLevel(GameState* state) {
  try {
    std::ifstream fin{state->curLevel};
    if (!fin.good()) {
      throw std::invalid_argument{
          ("Unable to open level file \"" + state->curLevel + "\"").c_str()};
    }
    nlohmann::json json = nlohmann::json::parse(fin);

    auto palette = json["palette"];

    auto tiles = json["tiles"];
    state->board.resize(tiles.size());
    for (size_t x = 0; x < tiles.size(); ++x) {
      state->board[x].resize(tiles[x].size());
      if (tiles[x].size() != tiles[0].size()) {
        printf(
            "Warning: tiles[0] has length %zu but tiles[%zu] has length %zu\n",
            tiles[0].size(), x, tiles[x].size());
      }
      for (size_t y = 0; y < tiles[x].size(); ++y) {
        state->board[x][y].resize(tiles[x][y].size());
        for (size_t i = 0; i < tiles[x][y].size(); ++i) {
          state->board[x][y][i] = jsonToTile(
              state, &palette[tiles[x][y][i].get<size_t>()], x, y, i);
        }
      }
    }
  } catch (std::exception& e) {
    printf("An exception ocurred while loading level:\n  %s\n", e.what());
    clearBoard(state);
    puts("Loading fallback level\n");
    loadFallbackLevel(state);
  }
}

void setupBoard(GameState* state) {
  loadLevel(state);
  state->history.clear();

  // resources dependent on level data
  size_t width = state->board.size();
  size_t height = width ? state->board[0].size() : 0;

  state->bulletsRenderTexture.create((unsigned int)(width * TILE_SIZE),
                                     (unsigned int)(height * TILE_SIZE));
  state->bulletsSprite.setTexture(state->bulletsRenderTexture.getTexture(),
                                  true);
  state->bulletsShader1.setUniform("target",
                                   state->bulletsRenderTexture.getTexture());
  state->bulletsShader1.setUniform(
      "boardSize",
      sf::Glsl::Vec2{(float)(width * TILE_SIZE), (float)(height * TILE_SIZE)});
}

void initialize(GameState* state) {
  state->curLevel = "resources/levels/level0.json";
  setupBoard(state);
}

void undoBoard(GameState* state) {
  if (state->history.empty()) {
    return;
  }
  clearBoard(state);
  state->board.resize(state->history.back().size());
  for (size_t x = 0; x < state->history.back().size(); ++x) {
    state->board[x].resize(state->history.back()[x].size());
    for (size_t y = 0; y < state->history.back()[x].size(); ++y) {
      state->board[x][y].resize(state->history.back()[x][y].size());
      for (size_t i = 0; i < state->history.back()[x][y].size(); ++i) {
        state->board[x][y][i] = state->history.back()[x][y][i].restore(
            state, x, y, i, &state->history.back()[x][y][i].data);
      }
    }
  }
  state->history.pop_back();
}

void handleEvent(GameState* state, sf::Event* event) {
  switch (event->type) {
    case sf::Event::Closed:
      state->window->close();
      break;
    case sf::Event::KeyPressed:
      state->input.presses.push(event->key.code);
      if (event->key.code == sf::Keyboard::R) {
        clearBoard(state);
        setupBoard(state);
      }
      if (event->key.code == sf::Keyboard::Z) {
        undoBoard(state);
      }
      break;
    default:
      // NO OP
      break;
  }
}

void update(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        state->board[x][y][i]->update(state, x, y, i);
      }
    }
  }
}

void render(GameState* state) {
  state->bulletsRenderTexture.clear({0, 127, 0, 0});

  // naive implementation of z ordering: collect all tiles and sort by z
  std::vector<std::tuple<Tile*, size_t, size_t, size_t>> zOrderedTiles;

  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        zOrderedTiles.push_back({state->board[x][y][i], x, y, i});
      }
    }
  }
  std::sort(zOrderedTiles.begin(), zOrderedTiles.end(),
            [state](std::tuple<Tile*, size_t, size_t, size_t>& l,
                    std::tuple<Tile*, size_t, size_t, size_t>& r) {
              return std::get<0>(l)->getZLayer(state, std::get<1>(l),
                                               std::get<2>(l), std::get<3>(l)) <
                     std::get<0>(r)->getZLayer(state, std::get<1>(r),
                                               std::get<2>(r), std::get<3>(r));
            });
  for (std::tuple<Tile*, size_t, size_t, size_t> tile : zOrderedTiles) {
    std::get<0>(tile)->render(state, std::get<1>(tile), std::get<2>(tile),
                              std::get<3>(tile));
  }

  state->bulletsRenderTexture.display();
  state->window->draw(state->bulletsSprite, &state->bulletsShader2);
}

void uninitialize(GameState* state) { clearBoard(state); }
