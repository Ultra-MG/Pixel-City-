#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "game/BuildTool.hpp"

enum class BuildCategory {
  Infrastructure,
  Building,
  Decorations
};

struct BuildEntry {
  BuildCategory category;
  BuildTool tool;
  const char* label;
  const char* iconPath;
};

inline const std::vector<BuildEntry>& buildCatalog() {
  static const std::vector<BuildEntry> entries = {
    { BuildCategory::Infrastructure, BuildTool::PlaceRoad,  "Road",  "assets/road.png"  },
    { BuildCategory::Building,       BuildTool::PlaceHouse, "House", "assets/house.png" },
    { BuildCategory::Decorations,          BuildTool::PlaceFountain, "Fountain", "assets/Decorations/fountain.png" }
  };
  return entries;
}
