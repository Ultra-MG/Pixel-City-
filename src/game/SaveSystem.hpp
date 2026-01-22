#pragma once

#include "game/SaveTypes.hpp"
#include <optional>
#include <string>
#include <vector>

class City;
class Wallet;

namespace SaveSystem {

GameState buildState(const std::string& cityName,
                     const City& city,
                     const Wallet& wallet);

bool applyState(const GameState& state,
                City& city,
                Wallet& wallet);

bool saveState(const GameState& state);
std::optional<GameState> loadState(const std::string& fileName);

std::vector<SaveInfo> listSaves();
std::string sanitizeName(const std::string& name);

}
