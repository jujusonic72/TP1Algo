
#include "Items/ItemDamageBoost.h"

#include "Player.h"

void ItemDamageBoost::UseItem(Player* player)
{

    // Logique pour augmenter les dégâts du joueur
    if (!player) {
        return;
    }
    player->applyDamageBoost(value_, duration_ms);
}
