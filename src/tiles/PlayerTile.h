#pragma once

#include "Tile.h"

/// @brief Contains logic for player movement
class PlayerTile : public Tile {
 private:
  sf::Sprite deathSprite;
  size_t moveToX;
  size_t moveToY;
  sf::Time startedMoving;
  bool alive = true;

  /// @brief Checks if the player's position is safe. If not, modifies the state
  void checkAlive(GameState* state);

 public:
  PlayerTile(sf::Sprite sprite, sf::Sprite deathSprite, size_t x, size_t y);
  PlayerTile(GameState* state, nlohmann::json* json, size_t x, size_t y,
             size_t i);

  void update(GameState* state, size_t x, size_t y, size_t i) override;
  void render(GameState* state, size_t x, size_t y, size_t i) override;
  int getZLayer(GameState* state, size_t x, size_t y, size_t i) const override;
  void prepareMove(GameState* state, size_t x, size_t y, size_t i) override;
  bool finishMove(GameState* state, size_t x, size_t y, size_t i) override;
  SerializedTile serialize() const override;
};
