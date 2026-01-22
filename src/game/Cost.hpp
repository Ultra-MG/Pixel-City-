#pragma once

enum class Currency {
    Money,
    Diamonds
};

struct Cost {
    Currency currency;
    int amount;

    constexpr Cost(Currency c = Currency::Money, int a = 0)
        : currency(c), amount(a) {}
};
