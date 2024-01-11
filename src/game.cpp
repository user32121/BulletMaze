#include "game.h"

#include <SFML/Graphics.hpp>

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

// TODO selecting different levels
void setupBoard(GameState* state) {
  srand(1);

  constexpr size_t width = 7;
  constexpr size_t height = 5;

  state->bulletsRenderTexture.create(width * TILE_SIZE, height * TILE_SIZE);
  state->bulletsSprite.setTexture(state->bulletsRenderTexture.getTexture(),
                                  true);
  state->bulletsShader1.setUniform("target",
                                   state->bulletsRenderTexture.getTexture());
  state->bulletsShader1.setUniform(
      "boardSize", sf::Glsl::Vec2{width * TILE_SIZE, height * TILE_SIZE});

  // floor
  static std::string floorTextures[4] = {
      "resources/textures/floor0.png",
      "resources/textures/floor1.png",
      "resources/textures/floor2.png",
      "resources/textures/floor3.png",
  };
  state->board.resize(7);
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

  // spawners
  state->board[width - 1][height - 1].push_back(
      new BulletSpawnerTile<StraightBulletTile>{
          state->textureManager.getSprite("resources/textures/spawner.png"),
          [](GameState* state, size_t x, size_t y) {
            return new StraightBulletTile{state->textureManager.getSprite(
                                              "resources/textures/bulletL.png"),
                                          x, y, LEFT};
          },
          5});
  state->board[0][height - 1].push_back(
      new BulletSpawnerTile<StraightBulletTile>{
          state->textureManager.getSprite("resources/textures/spawner.png"),
          [](GameState* state, size_t x, size_t y) {
            return new StraightBulletTile{state->textureManager.getSprite(
                                              "resources/textures/bulletR.png"),
                                          x, y, RIGHT};
          },
          5});
  state->board[width - 2][0].push_back(
      new BulletSpawnerTile<StraightBulletTile>{
          state->textureManager.getSprite("resources/textures/spawner.png"),
          [](GameState* state, size_t x, size_t y) {
            return new StraightBulletTile{state->textureManager.getSprite(
                                              "resources/textures/bulletD.png"),
                                          x, y, DOWN, -1};
          },
          4, 1});

  state->history.clear();
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

void initialize(GameState* state) { setupBoard(state); }

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
