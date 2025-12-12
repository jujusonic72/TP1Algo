#include "../../include/Commands/CancelCommand.h"

void CancelCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    queueHandler->nextCommand();
}

void CancelCommand::execute(Uint64 delta_time)
{
    // Nothing to do here since the command is already handled in pre_execute
}