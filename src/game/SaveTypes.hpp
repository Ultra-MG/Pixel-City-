#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "world/Tile.hpp"
#include "game/BuildTool.hpp"
#include <unordered_map>

struct PlacedObject
{
    BuildTool tool;
    int x = 0;
    int y = 0;

    std::unordered_map<std::string, std::string> data;
};


struct GameState {
    std::string cityName;

    int money = 0;
    int diamonds = 0;

    int w = 0;
    int h = 0;
    std::vector<Tile> tiles;
    std::vector<PlacedObject> objects;
    std::unordered_map<std::string, int> inventory;

    std::int64_t lastSaveTimestamp = 0;
};

struct SaveInfo {
    std::string displayName;
    std::string fileName;
};
