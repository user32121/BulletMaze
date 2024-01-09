#include "game.h"

#include <SFML/Graphics.hpp>

#include "TextureManager.h"
#include "tiles/BulletSpawnerTile.h"
#include "tiles/PlayerTile.h"
#include "tiles/SolidTile.h"
#include "tiles/StraightBulletTile.h"

// TODO programmatically generated texture atlas
inline sf::Sprite getSprite(GameState* state, int x, int y) {
  return {state->spriteSheet,
          sf::IntRect(TILE_SIZE * x, TILE_SIZE * y, TILE_SIZE, TILE_SIZE)};
}

void loadResources(GameState* state) {
  state->spriteSheet.loadFromFile("resources/SpriteSheet.png");

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
  state->board.resize(7);
  for (size_t x = 0; x < width; ++x) {
    state->board[x].resize(height);
    for (size_t y = 0; y < height; y++) {
      state->board[x][y].push_back(new Tile{getSprite(state, rand() % 4, 0)});
    }
  }
  // player
  state->board[0][0].push_back(
      new PlayerTile{getSprite(state, 4, 0), getSprite(state, 11, 0), 0, 0});

  // random obstacles
  for (size_t i = 0; i < 3; ++i) {
    state->board[rand() % width][rand() % height].push_back(
        new SolidTile{getSprite(state, 5, 0)});
  }

  // spawners
  state->board[width - 1][height - 1].push_back(
      new BulletSpawnerTile<StraightBulletTile>(
          getSprite(state, 10, 0), 5,
          std::function{[](GameState* state, size_t x, size_t y) {
            return new StraightBulletTile{getSprite(state, 6, 0), x, y, LEFT};
          }}));
  state->board[0][height - 1].push_back(
      new BulletSpawnerTile<StraightBulletTile>(
          getSprite(state, 10, 0), 5,
          std::function{[](GameState* state, size_t x, size_t y) {
            return new StraightBulletTile{getSprite(state, 7, 0), x, y, RIGHT};
          }}));
  state->board[width - 2][0].push_back(new BulletSpawnerTile<
                                       StraightBulletTile>(
      getSprite(state, 10, 0), 4, 2,
      std::function{[](GameState* state, size_t x, size_t y) {
        return new StraightBulletTile{getSprite(state, 9, 0), x, y, DOWN, -1};
      }}));
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
      // TODO Z undo

      if (event->key.code == sf::Keyboard::Q) {
        state->textureManager.loadTexture("resources/textures/player.png");
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

  sf::Sprite managerSprite{state->textureManager.spriteSheet.getTexture()};
  managerSprite.setPosition(100, 100);
  state->window->draw(managerSprite);
}

void uninitialize(GameState* state) { clearBoard(state); }
