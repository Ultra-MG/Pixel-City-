#pragma once
#include <vector>
#include <cstdint>

enum class Tile : std::uint8_t { Grass = 0, Road };

enum class BuildingType : std::uint8_t { House = 0 };

struct Building {
  BuildingType type = BuildingType::House;
  int x = 0, y = 0;   // top-left tile
  int w = 2, h = 2;   // size in tiles
  int level = 1;
};

class City {
public:
  City(int w, int h);

  int w() const { return m_w; }
  int h() const { return m_h; }

  Tile getTile(int x, int y) const;
  void setTile(int x, int y, Tile t);

  const std::vector<Building>& buildings() const { return m_buildings; }

  // --- Placement rules ---
  bool hasBuildingAt(int tx, int ty) const;               // any building covers this tile?
  bool canPlaceRoadAt(int tx, int ty) const;              // road must not overlap building
  bool canPlaceBuilding(const Building& b) const;         // building must not overlap building OR roads, AND must have road access
  void placeBuilding(const Building& b);

private:
  int m_w = 0;
  int m_h = 0;
  std::vector<Tile> m_tiles;
  std::vector<Building> m_buildings;

  bool inBounds(int x, int y) const;
  bool rectInBounds(int x, int y, int w, int h) const;
  bool rectOverlapsBuilding(int x, int y, int w, int h) const;
  bool rectHasRoad(int x, int y, int w, int h) const;

  // NEW: connectivity rule
  bool buildingHasRoadAccess(const Building& b) const;
};
