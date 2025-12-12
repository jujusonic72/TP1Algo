#pragma once
#include "../QueueHandler.h"
#include "../Command.h"

class CancelCommand : public Command
{
public:
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
};
