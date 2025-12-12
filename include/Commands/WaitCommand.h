#pragma once
#include "../QueueHandler.h"
#include "../Command.h"

class WaitCommand : public Command
{
public:
    WaitCommand();
    WaitCommand(int durationMs) {waitDurationMs = durationMs;}
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
    void post_execute() override;
protected:
    int waitDurationMs;
    int elapsedWaitTime = 0;
};
