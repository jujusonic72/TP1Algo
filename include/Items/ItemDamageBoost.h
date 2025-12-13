#pragma once
#include "Item.h"

class Player;
class ItemDamageBoost : public Item
{
public:
    ItemDamageBoost(const std::string& name, const std::string& description, int value, Uint64 duration)
        : Item(name, description, value), duration_ms(duration)
    {
        type_ = ItemType::DAMAGE_BOOST;

    };

    void UseItem(Player* player) override;

private:
    Uint64 duration_ms; // Durée de l'effet en millisecondes
};

