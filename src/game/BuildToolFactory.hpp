#pragma once

#include "game/BuildTool.hpp"
#include "world/Placeable.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

class BuildToolFactory {
public:
    using Creator = std::function<std::unique_ptr<Placeable>(int, int)>;

    static BuildToolFactory& instance();

    void registerTool(BuildTool tool, Creator creator);
    std::unique_ptr<Placeable> create(BuildTool tool, int x, int y) const;

private:
    std::unordered_map<BuildTool, Creator> m_creators;
};
