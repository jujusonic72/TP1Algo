#pragma once
#include "../QueueHandler.h"
#include "../Command.h"
#include "../Position.h"

class MoveCommand : public Command
{
public:
    MoveCommand();
    MoveCommand(Position targetPos){targetPosition = targetPos;}
    void pre_execute(QueueHandler* queueHandler) override;
    void execute(Uint64 delta_time) override;
    void post_execute() override;
    void set_force_finish(bool b) override { force_finish_ = b; }
    void set_finished(bool b) override { finished_ = b; }
    void reached_target();
    void update_position(Uint64 delta_time);
    Position targetPosition;
    Player* player = nullptr;
};
