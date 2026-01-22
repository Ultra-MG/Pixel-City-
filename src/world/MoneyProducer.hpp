#pragma once
#include <cstdint>
#include "game/EconomySystem.hpp"

class MoneyProducer
{
public:
    virtual ~MoneyProducer() = default;

    virtual int moneyPerMinute() const = 0;

    virtual int storedMoney() const = 0;
    virtual void setStoredMoney(int v) = 0;
    virtual void tick(std::int64_t seconds, EconomySystem &eco) = 0;

    virtual void applyOffline(std::int64_t seconds)
    {
        const int produced =
            static_cast<int>((seconds * moneyPerMinute()) / 60);
        if (produced > 0)
            setStoredMoney(storedMoney() + produced);
    }
};
