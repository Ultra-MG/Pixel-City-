// world/MoneyProducer.hpp (MODIFIED)
#pragma once
#include <cstdint>

class MoneyProducer
{
public:
    virtual ~MoneyProducer() = default;

    virtual int moneyPerMinute() const = 0;

    virtual int storedMoney() const = 0;
    virtual void setStoredMoney(int v) = 0;
    virtual int maxStorage() const { return 0; }

    virtual int baseStorage() const { return 0; }
    virtual int storagePerLevel() const { return 0; }

    virtual void tick(std::int64_t seconds)
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

    int collect()
    {
        const int v = storedMoney();
        setStoredMoney(0);
        return v;
    }

    virtual void applyOffline(std::int64_t seconds)
    {
        tick(seconds);
    }

private:
    std::int64_t m_fractional = 0;
};
