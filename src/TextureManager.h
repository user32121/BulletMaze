#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#define TILE_SIZE 64U

/// @brief keeps track of textures and aggregates them into a spritesheet.
/// Assumes all textures are of size TILE_SIZE
class TextureManager {
 private:
 public:  // TODO remove
  sf::RenderTexture spriteSheet;
  std::map<std::string, sf::Vector2u> locations;  // in pixels
  size_t size = 0;

  /// @brief ensures the texture is loaded
  void loadTexture(const std::string& filename);

  void increaseSize();

 public:
  TextureManager(size_t capacity = 1);

  TextureManager(const TextureManager& other) = delete;
  TextureManager& operator=(const TextureManager& other) = delete;
  ~TextureManager() = default;

  /// @brief returns a sprite that draws the corresponding texture specified by
  /// the filename
  sf::Sprite getSprite(const std::string& filename);
};
