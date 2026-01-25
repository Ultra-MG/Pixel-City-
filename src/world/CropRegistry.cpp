#include "world/CropRegistry.hpp"
#include "world/Crops/Wheat.hpp"
#include "world/Crops/Corn.hpp"
#include "world/Crops/Carrot.hpp"
#include "world/Crops/Potato.hpp"
#include <unordered_map>

const std::vector<CropInfo> &CropRegistry::all()
{
    static const std::vector<CropInfo> crops = {
        {"wheat", "Wheat", "assets/crops/wheat.png", 5, []() { return new Wheat(); }},
        {"corn", "Corn", "assets/crops/corn.png", 6, []() { return new Corn(); }},
        {"carrot", "Carrot", "assets/crops/carrot.png", 7, []() { return new Carrot(); }},
        {"potato", "Potato", "assets/crops/potato.png", 8, []() { return new Potato(); }}};
    return crops;
}

Crop *CropRegistry::create(const std::string &id)
{
    static const std::unordered_map<std::string, std::function<Crop *()>> map = []()
    {
        std::unordered_map<std::string, std::function<Crop *()>> out;
        for (const auto &c : CropRegistry::all())
            out.emplace(c.id, c.create);
        return out;
    }();

    auto it = map.find(id);
    if (it == map.end())
        return nullptr;
    return it->second();
}

const CropInfo *CropRegistry::find(const std::string &id)
{
    static const std::unordered_map<std::string, const CropInfo *> map = []()
    {
        std::unordered_map<std::string, const CropInfo *> out;
        for (const auto &c : CropRegistry::all())
            out.emplace(c.id, &c);
        return out;
    }();

    auto it = map.find(id);
    if (it == map.end())
        return nullptr;
    return it->second;
}
