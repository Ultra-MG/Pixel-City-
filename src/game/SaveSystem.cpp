#include "game/SaveSystem.hpp"
#include "game/BuildToolFactory.hpp"
#include "world/Infrastructure/CropField.hpp"
#include "world/Infrastructure/Road.hpp"
#include "world/Infrastructure/Water.hpp"
#include "world/Infrastructure/Bridge.hpp"
#include "world/Buildings/House.hpp"
#include "world/Buildings/Farm.hpp"
#include "world/Buildings/Store.hpp"
#include "world/Buildings/TownHall.hpp"
#include "world/Decorations/Fountain.hpp"
#include "world/Decorations/Tree.hpp"
#include "world/Decorations/Bench.hpp"
#include "world/Decorations/LampPost.hpp"
#include "world/Crops/Wheat.hpp"
#include "world/Crops/Corn.hpp"
#include "world/Crops/Carrot.hpp"
#include "world/Crops/Potato.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{
BuildTool toolFromPlaceable(const Placeable &p)
{
    if (dynamic_cast<const House *>(&p))
        return BuildTool::PlaceHouse;
    if (dynamic_cast<const Road *>(&p))
        return BuildTool::PlaceRoad;
    if (dynamic_cast<const Fountain *>(&p))
        return BuildTool::PlaceFountain;
    if (dynamic_cast<const TownHall *>(&p))
        return BuildTool::PlaceTownHall;
    if (dynamic_cast<const Farm *>(&p))
        return BuildTool::PlaceFarm;
    if (dynamic_cast<const Store *>(&p))
        return BuildTool::PlaceStore;
    if (dynamic_cast<const Tree *>(&p))
        return BuildTool::PlaceTree;
    if (dynamic_cast<const LampPost *>(&p))
        return BuildTool::PlaceLampPost;
    if (dynamic_cast<const Bench *>(&p))
        return BuildTool::PlaceBench;
    if (dynamic_cast<const Water *>(&p))
        return BuildTool::PlaceWater;
    if (dynamic_cast<const Bridge *>(&p))
        return BuildTool::PlaceBridge;
    if (dynamic_cast<const CropField *>(&p))
        return BuildTool::PlaceCropField;

    return BuildTool::None;
}

std::unique_ptr<Crop> cropFromName(const std::string &name)
{
    if (name == "wheat")
        return std::make_unique<Wheat>();
    if (name == "corn")
        return std::make_unique<Corn>();
    if (name == "carrot")
        return std::make_unique<Carrot>();
    if (name == "potato")
        return std::make_unique<Potato>();
    return nullptr;
}

std::string cropNameFromField(const CropField &field)
{
    const Crop *c = field.crop();
    if (!c)
        return "";
    return c->typeName();
}

std::filesystem::path saveDir()
{
    return std::filesystem::path("saves");
}
} // namespace

GameState SaveSystem::buildState(const std::string &cityName, const City &city, const Wallet &wallet)
{
    GameState s;
    s.cityName = cityName;
    s.money = wallet.money();
    s.diamonds = wallet.diamonds();
    s.w = city.w();
    s.h = city.h();
    s.tiles.resize(static_cast<size_t>(s.w) * static_cast<size_t>(s.h));
    for (int y = 0; y < s.h; ++y)
    {
        for (int x = 0; x < s.w; ++x)
        {
            s.tiles[static_cast<size_t>(y) * static_cast<size_t>(s.w) + static_cast<size_t>(x)] = city.getTile(x, y);
        }
    }

    for (const auto &o : city.objects())
    {
        PlacedObject po;
        po.tool = toolFromPlaceable(*o);
        po.x = o->x;
        po.y = o->y;
        if (po.tool == BuildTool::PlaceCropField)
        {
            const auto *field = dynamic_cast<const CropField *>(o.get());
            if (field)
                po.cropType = cropNameFromField(*field);
        }
        s.objects.push_back(po);
    }
    return s;
}

bool SaveSystem::applyState(const GameState &state, City &city, Wallet &wallet)
{
    if (state.w <= 0 || state.h <= 0)
        return false;
    wallet.set(state.money, state.diamonds);

    city = City(state.w, state.h);
    if (state.tiles.size() == static_cast<size_t>(state.w) * static_cast<size_t>(state.h))
    {
        for (int y = 0; y < state.h; ++y)
        {
            for (int x = 0; x < state.w; ++x)
            {
                city.setTile(x, y, state.tiles[static_cast<size_t>(y) * static_cast<size_t>(state.w) + static_cast<size_t>(x)]);
            }
        }
    }

    for (const auto &o : state.objects)
    {
        if (o.tool == BuildTool::None)
            continue;
        std::unique_ptr<Placeable> obj = BuildToolFactory::instance().create(o.tool, o.x, o.y);
        if (!obj)
            continue;
        obj->x = o.x;
        obj->y = o.y;
        if (o.tool == BuildTool::PlaceCropField && !o.cropType.empty())
        {
            auto *field = dynamic_cast<CropField *>(obj.get());
            if (field)
            {
                auto crop = cropFromName(o.cropType);
                if (crop)
                    field->plantCrop(crop.release());
            }
        }
        city.place(std::move(obj));
    }

    return true;
}

bool SaveSystem::saveState(const GameState &state)
{
    const std::string safe = sanitizeName(state.cityName);
    if (safe.empty())
        return false;

    std::filesystem::create_directories(saveDir());
    const auto path = saveDir() / (safe + ".save");
    std::ofstream out(path, std::ios::trunc);
    if (!out)
        return false;

    out << "NAME " << state.cityName << "\n";
    out << "MONEY " << state.money << "\n";
    out << "DIAMONDS " << state.diamonds << "\n";
    out << "SIZE " << state.w << " " << state.h << "\n";
    out << "TILES\n";
    for (int y = 0; y < state.h; ++y)
    {
        for (int x = 0; x < state.w; ++x)
        {
            const Tile t = state.tiles[static_cast<size_t>(y) * static_cast<size_t>(state.w) + static_cast<size_t>(x)];
            out << static_cast<int>(t) << " ";
        }
        out << "\n";
    }
    out << "OBJECTS " << state.objects.size() << "\n";
    for (const auto &o : state.objects)
    {
        out << "OBJ " << static_cast<int>(o.tool) << " " << o.x << " " << o.y << " ";
        if (o.cropType.empty())
            out << "-";
        else
            out << o.cropType;
        out << "\n";
    }
    out << "END\n";
    return true;
}

std::optional<GameState> SaveSystem::loadState(const std::string &fileName)
{
    const auto path = saveDir() / fileName;
    std::ifstream in(path);
    if (!in)
        return std::nullopt;

    GameState state;
    std::string token;
    while (in >> token)
    {
        if (token == "NAME")
        {
            std::string rest;
            std::getline(in, rest);
            if (!rest.empty() && rest[0] == ' ')
                rest.erase(0, 1);
            state.cityName = rest;
        }
        else if (token == "MONEY")
        {
            in >> state.money;
        }
        else if (token == "DIAMONDS")
        {
            in >> state.diamonds;
        }
        else if (token == "SIZE")
        {
            in >> state.w >> state.h;
            state.tiles.resize(static_cast<size_t>(state.w) * static_cast<size_t>(state.h));
        }
        else if (token == "TILES")
        {
            for (int y = 0; y < state.h; ++y)
            {
                for (int x = 0; x < state.w; ++x)
                {
                    int v = 0;
                    in >> v;
                    state.tiles[static_cast<size_t>(y) * static_cast<size_t>(state.w) + static_cast<size_t>(x)] =
                        static_cast<Tile>(v);
                }
            }
        }
        else if (token == "OBJECTS")
        {
            size_t count = 0;
            in >> count;
            state.objects.clear();
            for (size_t i = 0; i < count; ++i)
            {
                std::string objToken;
                in >> objToken;
                if (objToken != "OBJ")
                    break;

                int toolInt = 0;
                PlacedObject po;
                in >> toolInt >> po.x >> po.y >> po.cropType;
                po.tool = static_cast<BuildTool>(toolInt);
                if (po.cropType == "-")
                    po.cropType.clear();
                state.objects.push_back(po);
            }
        }
        else if (token == "END")
        {
            break;
        }
    }

    if (state.w <= 0 || state.h <= 0)
        return std::nullopt;
    return state;
}

std::vector<SaveInfo> SaveSystem::listSaves()
{
    std::vector<SaveInfo> out;
    const auto dir = saveDir();
    if (!std::filesystem::exists(dir))
        return out;

    for (const auto &entry : std::filesystem::directory_iterator(dir))
    {
        if (!entry.is_regular_file())
            continue;
        const auto path = entry.path();
        if (path.extension() != ".save")
            continue;

        SaveInfo info;
        info.fileName = path.filename().string();
        auto state = loadState(info.fileName);
        if (state && !state->cityName.empty())
            info.displayName = state->cityName;
        else
            info.displayName = path.stem().string();
        out.push_back(info);
    }
    return out;
}

std::string SaveSystem::sanitizeName(const std::string &name)
{
    std::string out;
    for (char c : name)
    {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_' || c == '-')
        {
            out.push_back(c);
        }
        else if (c == ' ')
        {
            out.push_back('_');
        }
    }
    if (out.empty())
        out = "save";
    return out;
}
