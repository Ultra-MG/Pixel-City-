#pragma once
#include "game/BuildTool.hpp"

enum class Currency
{
    Money,
    Diamonds
};

struct Cost
{
    Currency currency = Currency::Money;
    int amount = 0;
};

class Wallet
{
public:
    Wallet(int money, int diamonds);

    int money() const { return m_money; }
    int diamonds() const { return m_diamonds; }

    bool canAfford(const Cost &cost) const;
    bool spend(const Cost &cost);
    void set(int money, int diamonds);

private:
    int m_money = 0;
    int m_diamonds = 0;
};

Cost buildCost(BuildTool tool);
