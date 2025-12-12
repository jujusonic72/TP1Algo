#include "../../include/Commands/WaitCommand.h"

#include <iostream>

void WaitCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;

    queueHandler->enqueue(this);
}

void WaitCommand::execute(Uint64 delta_time)
{
    if (force_finish_) {
        post_execute();
        return;
    }
    
    elapsedWaitTime += delta_time;
    std::cout << "Remaining time: " << waitDurationMs - elapsedWaitTime << "ms\n";
    if (elapsedWaitTime >= waitDurationMs) {
        set_finished(true);
    }

    if (finished_)
    {
        post_execute();
    }
}

void WaitCommand::post_execute()
{
    std::cout << "Done waiting\n";
    queueHandler->nextCommand();
}