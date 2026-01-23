#pragma once
#include "game/BuildTool.hpp"
#include "game/Cost.hpp"
class Wallet
{
public:
    Wallet(int money, int diamonds);

    void addMoney(int v) { m_money += v; }
    void addDiamonds(int v) { m_diamonds += v; }

    int money() const { return m_money; }
    int diamonds() const { return m_diamonds; }

    bool canAfford(const Cost &cost) const;
    bool spend(const Cost &cost);
    void set(int money, int diamonds);
    void add(const Cost &cost)
    {
        if (cost.amount <= 0)
            return;

        switch (cost.currency)
        {
        case Currency::Money:
            addMoney(cost.amount);
            break;
        case Currency::Diamonds:
            addDiamonds(cost.amount);
            break;
        }
    }

private:
    int m_money = 0;
    int m_diamonds = 0;
};

Cost buildCost(BuildTool tool);
