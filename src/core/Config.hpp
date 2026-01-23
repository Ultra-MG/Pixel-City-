#pragma once
#include <cstdint>

namespace cfg
{
  inline constexpr std::uint32_t InternalW = 320;
  inline constexpr std::uint32_t InternalH = 180;

  inline constexpr std::uint32_t Scale = 4;
  inline constexpr std::uint32_t WindowW = InternalW * Scale;
  inline constexpr std::uint32_t WindowH = InternalH * Scale;

  inline constexpr int TileSize = 8;

  // City size in tiles
  inline constexpr int CityW = 400;
  inline constexpr int CityH = 400;

  // Camera controls
  inline constexpr float ZoomMin = 0.5f;
  inline constexpr float ZoomMax = 3.0f;
  inline constexpr float ZoomStep = 1.1f; // wheel zoom factor

  inline constexpr const char *FontPixel =
      "assets/fonts/pixelFont.ttf";
}
