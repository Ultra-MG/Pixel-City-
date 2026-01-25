#pragma once
#include <cstdint>

namespace cfg
{
  inline constexpr std::uint32_t InternalW = 520;
  inline constexpr std::uint32_t InternalH = 340;

  inline constexpr std::uint32_t Scale = 4;
  inline constexpr std::uint32_t WindowW = InternalW * Scale;
  inline constexpr std::uint32_t WindowH = InternalH * Scale;

  inline constexpr int TileSize = 8;

  // City size in tiles
  inline constexpr int CityW = 600;
  inline constexpr int CityH = 600;

  // Camera controls
  inline constexpr float ZoomMin = 0.5f;
  inline constexpr float ZoomMax = 3.0f;
  inline constexpr float ZoomStep = 1.1f; 

  inline constexpr const char *FontPixel =
      "assets/fonts/pixelFont.ttf";
}
