#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "game/BuildTool.hpp"

enum class BuildCategory
{
  Infrastructure,
  Building,
  Decorations
};

struct BuildEntry
{
  BuildCategory category;
  BuildTool tool;
  const char *label;
  const char *iconPath;
};

inline const std::vector<BuildEntry> &buildCatalog()
{
  static const std::vector<BuildEntry> entries = {
      {BuildCategory::Infrastructure, BuildTool::PlaceRoad, "Road", "assets/road.png"},
      {BuildCategory::Infrastructure, BuildTool::PlaceBridge, "Bridge", "assets/infrastructure/bridge.png"},
      {BuildCategory::Infrastructure, BuildTool::PlaceWater, "Water", "assets/infrastructure/water.png"},
      {BuildCategory::Infrastructure, BuildTool::PlaceCropField, "Crop Field", "assets/infrastructure/cropfield.png"},
      {BuildCategory::Building, BuildTool::PlaceHouse, "House", "assets/house.png"},
      {BuildCategory::Building, BuildTool::PlaceStore, "Store", "assets/store.png"},
      {BuildCategory::Building, BuildTool::PlaceTownHall, "Town Hall", "assets/townhall.png"},
      {BuildCategory::Building, BuildTool::PlaceFarm, "Farm", "assets/farm.png"},
      {BuildCategory::Decorations, BuildTool::PlaceFountain, "Fountain", "assets/Decorations/fountain.png"},
      {BuildCategory::Decorations, BuildTool::PlaceTree, "Tree", "assets/decorations/tree.png"},
      {BuildCategory::Decorations, BuildTool::PlaceBench, "Bench", "assets/decorations/bench.png"},
      {BuildCategory::Decorations, BuildTool::PlaceLampPost, "Lamp Post", "assets/decorations/lamppost.png"}};
  return entries;
}
