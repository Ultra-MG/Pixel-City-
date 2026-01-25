#include "game/Wallet.hpp"

Wallet::Wallet(int money, int diamonds)
    : m_money(money), m_diamonds(diamonds)
{
}

bool Wallet::canAfford(const Cost &cost) const
{
    if (cost.amount <= 0)
        return true;

    switch (cost.currency)
    {
    case Currency::Money:
        return m_money >= cost.amount;
    case Currency::Diamonds:
        return m_diamonds >= cost.amount;
    }
    return false;
}

bool Wallet::spend(const Cost &cost)
{
    if (!canAfford(cost))
        return false;

    switch (cost.currency)
    {
    case Currency::Money:
        m_money -= cost.amount;
        break;
    case Currency::Diamonds:
        m_diamonds -= cost.amount;
        break;
    }
    return true;
}

void Wallet::set(int money, int diamonds)
{
    m_money = money;
    m_diamonds = diamonds;
}
