#include "game/SaveSystem.hpp"
#include "game/BuildToolFactory.hpp"
#include "world/Buildings/TownHall.hpp"
#include "world/City.hpp"
#include "game/Wallet.hpp"
#include "game/Inventory.hpp"
#include <filesystem>
#include <fstream>
#include <ctime>

namespace
{
    std::filesystem::path saveDir()
    {
        return std::filesystem::path("saves");
    }
}

namespace SaveSystem
{

    GameState buildState(const std::string &cityName,
                         const City &city,
                         const Wallet &wallet,
                         const Inventory &inv)
    {
        GameState s;
        s.cityName = cityName;
        s.money = wallet.money();
        s.diamonds = wallet.diamonds();
        s.w = city.w();
        s.h = city.h();
        s.lastSaveTimestamp = std::time(nullptr);

        s.tiles.resize(static_cast<size_t>(s.w) * static_cast<size_t>(s.h));
        for (int y = 0; y < s.h; ++y)
            for (int x = 0; x < s.w; ++x)
                s.tiles[y * s.w + x] = city.getTile(x, y);

        for (const auto &o : city.objects())
        {
            PlacedObject po;
            o->saveTo(po);
            s.objects.push_back(std::move(po));
        }

        s.inventory = inv.all();
        return s;
    }

    bool applyState(const GameState &state, City &city, Wallet &wallet, Inventory &inv)
    {
        if (state.w <= 0 || state.h <= 0)
            return false;

        wallet.set(state.money, state.diamonds);
        inv = Inventory();
        for (const auto &kv : state.inventory)
            inv.add(kv.first, kv.second);
        city = City(state.w, state.h);

        for (int y = 0; y < state.h; ++y)
            for (int x = 0; x < state.w; ++x)
                city.setTile(x, y, state.tiles[y * state.w + x]);

        for (const auto &o : state.objects)
        {
            if (o.tool == BuildTool::None)
            {
                continue;
            }
            auto obj = BuildToolFactory::instance().create(o.tool, o.x, o.y);
            if (!obj)
                continue;

            obj->loadFrom(o);
            city.place(std::move(obj));
        }

        return true;
    }

    bool saveState(const GameState &state)
    {
        const std::string safe = sanitizeName(state.cityName);
        if (safe.empty())
            return false;

        std::filesystem::create_directories(saveDir());
        std::ofstream out(saveDir() / (safe + ".save"), std::ios::trunc);
        if (!out)
            return false;

        out << "NAME " << state.cityName << "\n";
        out << "MONEY " << state.money << "\n";
        out << "DIAMONDS " << state.diamonds << "\n";
        out << "TIME " << state.lastSaveTimestamp << "\n";
        out << "SIZE " << state.w << " " << state.h << "\n";
        out << "TILES\n";

        for (int y = 0; y < state.h; ++y)
        {
            for (int x = 0; x < state.w; ++x)
                out << static_cast<int>(state.tiles[y * state.w + x]) << " ";
            out << "\n";
        }

        out << "OBJECTS " << state.objects.size() << "\n";

        for (const auto &o : state.objects)
        {
            out << "OBJ "
                << static_cast<int>(o.tool) << " "
                << o.x << " " << o.y << "\n";

            for (const auto &[k, v] : o.data)
                out << "DATA " << k << " " << v << "\n";

            out << "ENDOBJ\n";
        }

        out << "INVENTORY " << state.inventory.size() << "\n";
        for (const auto &kv : state.inventory)
            out << "ITEM " << kv.first << " " << kv.second << "\n";

        out << "END\n";
        return true;
    }

    std::optional<GameState> loadState(const std::string &fileName)
    {
        std::ifstream in(saveDir() / fileName);
        if (!in)
            return std::nullopt;

        GameState state;
        std::string token;

        while (in >> token)
        {
            if (token == "NAME")
            {
                std::getline(in, state.cityName);
                if (!state.cityName.empty() && state.cityName[0] == ' ')
                    state.cityName.erase(0, 1);
            }
            else if (token == "MONEY")
                in >> state.money;
            else if (token == "DIAMONDS")
                in >> state.diamonds;
            else if (token == "TIME")
                in >> state.lastSaveTimestamp;
            else if (token == "SIZE")
            {
                in >> state.w >> state.h;
                state.tiles.resize(state.w * state.h);
            }
            else if (token == "TILES")
            {
                for (int y = 0; y < state.h; ++y)
                    for (int x = 0; x < state.w; ++x)
                    {
                        int v;
                        in >> v;
                        state.tiles[y * state.w + x] = static_cast<Tile>(v);
                    }
            }
            else if (token == "OBJECTS")
            {
                size_t count;
                in >> count;
                for (size_t i = 0; i < count; ++i)
                {
                    PlacedObject po;
                    in >> token;
                    int tool;
                    in >> tool >> po.x >> po.y;
                    po.tool = static_cast<BuildTool>(tool);

                    while (in >> token && token != "ENDOBJ")
                    {
                        if (token == "DATA")
                        {
                            std::string k, v;
                            in >> k >> v;
                            po.data[k] = v;
                        }
                    }

                    state.objects.push_back(std::move(po));
                }
            }
            else if (token == "INVENTORY")
            {
                size_t count;
                in >> count;
                for (size_t i = 0; i < count; ++i)
                {
                    std::string itemToken;
                    in >> itemToken;
                    if (itemToken != "ITEM")
                        break;
                    std::string id;
                    int qty = 0;
                    in >> id >> qty;
                    state.inventory[id] = qty;
                }
            }
            else if (token == "END")
                break;
        }

        return state;
    }

    std::vector<SaveInfo> listSaves()
    {
        std::vector<SaveInfo> out;
        if (!std::filesystem::exists(saveDir()))
            return out;

        for (const auto &e : std::filesystem::directory_iterator(saveDir()))
        {
            if (e.path().extension() != ".save")
                continue;

            SaveInfo info;
            info.fileName = e.path().filename().string();
            auto st = loadState(info.fileName);
            info.displayName = st ? st->cityName : e.path().stem().string();
            out.push_back(info);
        }
        return out;
    }

    std::string sanitizeName(const std::string &name)
    {
        std::string out;
        for (char c : name)
        {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-')
                out.push_back(c);
            else if (c == ' ')
                out.push_back('_');
        }
        return out.empty() ? "save" : out;
    }

    bool deleteSave(const std::string &fileName)
    {
        const auto path = saveDir() / fileName;

        if (!std::filesystem::exists(path))
            return false;

        return std::filesystem::remove(path);
    }
}
