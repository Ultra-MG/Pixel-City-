#pragma once
#include <string>

struct CollectResult
{
    int money = 0;
    std::string itemId;
    int itemCount = 0;
    std::string popupText;
};

class Collectable
{
public:
    virtual ~Collectable() = default;
    virtual bool canCollect() const = 0;
    virtual CollectResult collect() = 0;
    virtual std::string collectIconId() const = 0;
};
