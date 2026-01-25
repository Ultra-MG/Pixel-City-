#pragma once
#include "game/Wallet.hpp"

class EconomySystem {
public:
    explicit EconomySystem(Wallet& wallet)
        : m_wallet(wallet) {}

    void addMoney(int amount) {
        if (amount > 0)
            m_wallet.addMoney(amount);
    }

private:
    Wallet& m_wallet;
};
