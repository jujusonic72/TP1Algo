#include "../../include/Commands/CancelCommand.h"

void CancelCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    if (position_to_cancel > 0)
    {
        queueHandler->removeCommandAtPosition(position_to_cancel);
    }
    else {
      queueHandler->nextCommand();
    }
}

void CancelCommand::execute(Uint64 delta_time)
{
    // Nothing to do here since the command is already handled in pre_execute
}