#include "game.h"

#include <SFML/Graphics.hpp>

#include "tiles/PlayerTile.h"

void loadResources(GameState* state) {
  state->spriteSheet.loadFromFile("resources/SpriteSheet.png");
}

void initialize(GameState* state) {
  // floor
  state->board.resize(7);
  for (size_t x = 0; x < 7; ++x) {
    state->board[x].resize(5);
    for (size_t y = 0; y < 5; y++) {
      Tile* t = new Tile{{state->spriteSheet,
                          {TILE_SIZE * (rand() % 4), 0, TILE_SIZE, TILE_SIZE}}};
      state->board[x][y].push_back(t);
    }
  }
  // player
  Tile* t = new PlayerTile{
      {state->spriteSheet, {TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE}}, 0, 0};
  state->board[0][0].push_back(t);
}

void handleEvent(GameState* state, sf::Event* event) {
  switch (event->type) {
    case sf::Event::Closed:
      state->window->close();
      break;
    case sf::Event::KeyPressed:
      state->input.presses.push(event->key.code);
      break;
  }
}

void update(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        bool moved = state->board[x][y][i]->update(state, x, y);
        if (moved) {
          --i;
        }
      }
    }
  }
}

void render(GameState* state) {
  // naive implementation of z ordering: collect all tiles and sort by z
  std::vector<std::tuple<Tile*, size_t, size_t>> zOrderedTiles;

  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        zOrderedTiles.push_back({state->board[x][y][i], x, y});
      }
    }
  }
  std::sort(zOrderedTiles.begin(), zOrderedTiles.end(),
            [state](std::tuple<Tile*, size_t, size_t>& l,
                    std::tuple<Tile*, size_t, size_t>& r) {
              return std::get<0>(l)->getZLayer(state, std::get<1>(l),
                                                std::get<2>(l)) <
                     std::get<0>(r)->getZLayer(state, std::get<1>(r),
                                                std::get<2>(r));
            });
  for (std::tuple<Tile*, size_t, size_t> tile : zOrderedTiles) {
    std::get<0>(tile)->render(state, std::get<1>(tile), std::get<2>(tile));
  }
}

void uninitialize(GameState* state) {
  for (size_t x = 0; x < state->board.size(); ++x) {
    for (size_t y = 0; y < state->board[x].size(); ++y) {
      for (size_t i = 0; i < state->board[x][y].size(); ++i) {
        delete state->board[x][y][i];
      }
    }
  }
}
