#pragma once
#include "../QueueHandler.h"
#include "../Command.h"

class TalkCommand : public Command
{
public:
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
    void post_execute() override;
protected:
    Player* player = nullptr;
};
