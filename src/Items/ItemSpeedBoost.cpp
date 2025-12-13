
#include "Items/ItemSpeedBoost.h"

#include "Player.h"

void ItemSpeedBoost::UseItem(Player* player)
{

    // Logique pour augmenter les dégâts du joueur
    if (!player) {
        return;
    }
    player->applySpeedBoost(value_, duration_ms);
}
