#include "game/BuildToolFactory.hpp"

BuildToolFactory& BuildToolFactory::instance()
{
    static BuildToolFactory f;
    return f;
}

void BuildToolFactory::registerTool(BuildTool tool, Creator creator)
{
    m_creators[tool] = std::move(creator);
}

std::unique_ptr<Placeable>
BuildToolFactory::create(BuildTool tool, int x, int y) const
{
    auto it = m_creators.find(tool);
    if (it == m_creators.end())
        return nullptr;

    return it->second(x, y);
}
