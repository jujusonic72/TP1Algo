#pragma once
#include "Item.h"

class Player;
class ItemHealthPotion : public Item
{
public:
    ItemHealthPotion(const std::string& name, const std::string& description, int value)
        : Item(name, description, value)
    {
        type_ = ItemType::HEALTH_POTION;

    };

    void UseItem(Player* player) override;
};