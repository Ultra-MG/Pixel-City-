#pragma once

#include "game/Economy.hpp"
#include "game/BuildTool.hpp"
#include "world/City.hpp"
#include "world/Tile.hpp"
#include <optional>
#include <string>
#include <vector>

struct PlacedObject
{
    BuildTool tool = BuildTool::None;
    int x = 0;
    int y = 0;
    std::string cropType;
};

struct GameState
{
    std::string cityName;
    int money = 0;
    int diamonds = 0;
    int w = 0;
    int h = 0;
    std::vector<Tile> tiles;
    std::vector<PlacedObject> objects;
};

struct SaveInfo
{
    std::string displayName;
    std::string fileName;
};

namespace SaveSystem
{
GameState buildState(const std::string &cityName, const City &city, const Wallet &wallet);
bool applyState(const GameState &state, City &city, Wallet &wallet);

bool saveState(const GameState &state);
std::optional<GameState> loadState(const std::string &fileName);
std::vector<SaveInfo> listSaves();
std::string sanitizeName(const std::string &name);
}
