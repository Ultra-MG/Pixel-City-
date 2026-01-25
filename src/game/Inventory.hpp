#pragma once
#include <string>
#include <unordered_map>

class Inventory
{
public:
    void add(const std::string &id, int count);
    bool remove(const std::string &id, int count);
    int count(const std::string &id) const;
    const std::unordered_map<std::string, int> &all() const { return m_items; }

private:
    std::unordered_map<std::string, int> m_items;
};
