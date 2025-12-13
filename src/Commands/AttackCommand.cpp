#include "../../include/Commands/AttackCommand.h"
#include "Enemy.h"

#include <iostream>

void AttackCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    this->player = queueHandler->get_player();

    queueHandler->enqueue(this);
}

void AttackCommand::execute(Uint64 delta_time)
{
    if (force_finish_) {
        post_execute();
        return;
    }
    
    elapsedAttackTime += delta_time;
    if (elapsedAttackTime >= attackTimeMs) {
        trigger_attack();

        set_finished(true);
    }

    if (finished_)
    {
        post_execute();
    }
}

void AttackCommand::post_execute()
{
    std::cout << "Done waiting\n";
    queueHandler->nextCommand();
}

void AttackCommand::trigger_attack()
{
    Enemy* closest_enemy = nullptr;

    for (auto& enemy : queueHandler->get_enemies())
    {
        float distance = sqrt(pow(enemy->getPosition().x - player->getPosition().x, 2) + pow(enemy->getPosition().y - player->getPosition().y, 2));
        if (distance <= player->getAttackRange())
        {
            if (closest_enemy == nullptr || distance < sqrt(pow(closest_enemy->getPosition().x - player->getPosition().x, 2) + pow(closest_enemy->getPosition().y - player->getPosition().y, 2)))
            {
                closest_enemy = enemy;
            }
        }
    }

    if (closest_enemy)
    {
        if(closest_enemy->takeDamage(player->getDamage()))
        {
            queueHandler->kill_enemy(closest_enemy);
            std::cout << "Enemy defeated!\n";
        }
        else
        {
          std::cout << "Enemy still standing!\n";

          player->SetHealth(player->getHealth() - closest_enemy->getDamage());
          std::cout << "Player took damage! Current health: " << player->getHealth() << "\n";

          if(player->getHealth() <= 0)
          {
              std::cout << "Player has been defeated! Game Over.\n";
              exit(0);
          }
        }
    }
    else
        std::cout << "No enemy in range to attack.\n";
}