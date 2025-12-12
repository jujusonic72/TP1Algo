#include "../../include/Commands/MoveCommand.h"

#include <iostream>

MoveCommand::MoveCommand() : targetPosition{0, 0} {}
void MoveCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    this->player = queueHandler->get_player();
    // Add action to queue
    queueHandler->enqueue(this);
}

void MoveCommand::execute(Uint64 delta_time)
{
    if (force_finish_) {
        post_execute();
        return;
    }
    
    // Implement movement logic here
    // Logique de mouvement
    update_position(delta_time);

    if (finished_)
    {
        post_execute();
    }
}

void MoveCommand::update_position(Uint64 delta_time)
{
    Position player_pos = player->getPosition();
    int speed = player->getEffectiveSpeed();
    float distance = sqrt(pow(targetPosition.x - player_pos.x, 2) + pow(targetPosition.y - player_pos.y, 2));
    if (distance == 0) {
        set_finished(true);
        return; // Déjà à la position cible
    }
    float dirx = (targetPosition.x - player_pos.x) / distance;
    float diry = (targetPosition.y - player_pos.y) / distance;
    float move_distance = speed * (delta_time / 1000.0f);
    if (move_distance >= distance) {
        player->SetPosition(targetPosition);
        set_finished(true);
        return;
    }
    //std::cout << "Move from {" << player_pos.x << "," << player_pos.y << "} to {" << player_pos.x + (dirx * move_distance) << "," << player_pos.y + (diry * move_distance) << "}" << "\n";
    player_pos.x += dirx * move_distance;
    player_pos.y += diry * move_distance;
    player->SetPosition(player_pos);
    
    reached_target();
}

void MoveCommand::reached_target()
{
    if (player->getPosition() == targetPosition)
    {
        std::cout << "Target reached\n";
        set_finished(true);
    }
}

void MoveCommand::post_execute()
{
    std::cout << "finish move command\n";
    queueHandler->nextCommand();
}
