#include "game/Inventory.hpp"

void Inventory::add(const std::string &id, int count)
{
    if (id.empty() || count <= 0)
        return;
    m_items[id] += count;
}

bool Inventory::remove(const std::string &id, int count)
{
    if (id.empty() || count <= 0)
        return false;
    auto it = m_items.find(id);
    if (it == m_items.end() || it->second < count)
        return false;
    it->second -= count;
    if (it->second <= 0)
        m_items.erase(it);
    return true;
}

int Inventory::count(const std::string &id) const
{
    auto it = m_items.find(id);
    if (it == m_items.end())
        return 0;
    return it->second;
}
