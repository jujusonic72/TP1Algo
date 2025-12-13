#pragma once
#include "../QueueHandler.h"
#include "../Command.h"

class UseCommand : public Command
{
public:
    UseCommand();
    UseCommand(int itemSlotIndex) { this->itemSlotIndex = itemSlotIndex; };
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
    void post_execute() override;
protected:
    int useTimeMs = 500;
    int elapsedUseTime = 0;
    Player* player = nullptr;

    int itemSlotIndex;

    void trigger_use();
};
