#pragma once
#include "../QueueHandler.h"
#include "../Command.h"

class AttackCommand : public Command
{
public:
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
    void post_execute() override;
protected:
    int attackTimeMs = 1000;
    int elapsedAttackTime = 0;
    Player* player = nullptr;

    void trigger_attack();
};
