#include "../../include/Commands/TalkCommand.h"
#include "QueueHandler.h"

#include <iostream>

void TalkCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    this->player = queueHandler->get_player();

    queueHandler->enqueue(this);
}

void TalkCommand::execute(Uint64 delta_time)
{
    if (force_finish_) {
        post_execute();
        return;
    }
    
    if (queueHandler->get_pnj()->isPlayerInRange(player->getPosition())) {
        queueHandler->get_pnj()->interact();
    }
    else {
        std::cout << "You are too far away to talk to the NPC." << "\n";
    }
    
    set_finished(true);

    if (finished_)
    {
        post_execute();
    }
}

void TalkCommand::post_execute()
{
    std::cout << "Done waiting\n";
    queueHandler->nextCommand();
}