#include "game/Economy.hpp"

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

Cost buildCost(BuildTool tool)
{
    switch (tool)
    {
    case BuildTool::PlaceRoad:
        return {Currency::Money, 5};
    case BuildTool::PlaceBridge:
        return {Currency::Money, 30};
    case BuildTool::PlaceWater:
        return {Currency::Money, 10};
    case BuildTool::PlaceCropField:
        return {Currency::Money, 40};
    case BuildTool::PlaceHouse:
        return {Currency::Money, 120};
    case BuildTool::PlaceStore:
        return {Currency::Money, 160};
    case BuildTool::PlaceTownHall:
        return {Currency::Diamonds, 5};
    case BuildTool::PlaceFarm:
        return {Currency::Money, 140};
    case BuildTool::PlaceFountain:
        return {Currency::Money, 50};
    case BuildTool::PlaceTree:
        return {Currency::Money, 10};
    case BuildTool::PlaceBench:
        return {Currency::Money, 15};
    case BuildTool::PlaceLampPost:
        return {Currency::Money, 20};
    case BuildTool::None:
        return {Currency::Money, 0};
    }
    return {Currency::Money, 0};
}
