#include "Items/ItemHealthPotion.h"

#include "Player.h"

void ItemHealthPotion::UseItem(Player* player)
{

    // Logique pour augmenter les dégâts du joueur
    if (!player) {
        return;
    }
    int new_health = player->getHealth() + value_;
    player->SetHealth(new_health);
}