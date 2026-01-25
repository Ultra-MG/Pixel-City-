#pragma once
#include <cstdint>
#include <algorithm>
#include <string>
#include "world/Collectable.hpp"

class Inventory;
class MoneyProducer : public Collectable
{
public:
    virtual ~MoneyProducer() = default;

    virtual int moneyPerMinute() const = 0;

    virtual int storedMoney() const = 0;
    virtual void setStoredMoney(int v) = 0;
    virtual int maxStorage() const { return 0; }

    virtual int baseStorage() const { return 0; }
    virtual int storagePerLevel() const { return 0; }
    virtual int collectThreshold() const { return 10; }

    bool canCollect() const override
    {
        return storedMoney() >= collectThreshold();
    }

    virtual void tick(std::int64_t seconds,Inventory& inventory )
    {
        m_fractional += seconds * moneyPerMinute();

        const int produced = m_fractional / 60;
        m_fractional %= 60;

        if (produced > 0)
        {
            const int capped = std::min(storedMoney() + produced, maxStorage());
            setStoredMoney(capped);
        }
    }

    CollectResult collect() override
    {
        if (!canCollect())
            return {};
        const int v = storedMoney();
        setStoredMoney(0);
        return CollectResult{v, "", 0, "+" + std::to_string(v)};
    }

    std::string collectIconId() const override
    {
        return "coin";
    }

    virtual void applyOffline(std::int64_t seconds,Inventory& inventory)
    {
        tick(seconds,inventory);
    }

private:
    std::int64_t m_fractional = 0;
};
