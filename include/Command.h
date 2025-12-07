#pragma once
#include <SDL3/SDL_stdinc.h>

#include "QueueHandler.h"

class Command
{
public:
    Command();
    virtual ~Command();
    virtual void pre_execute(QueueHandler* queueHandler) = 0;
    virtual void execute(Uint64 delta_time) = 0;
    virtual void post_execute();
    virtual void set_force_finish(bool b);
    virtual void set_finished(bool b);
    virtual bool is_edit_command() const { return is_edit_command_; }

protected:
    QueueHandler* queueHandler = nullptr;
    bool force_finish_ = false, finished_ = false, is_edit_command_ = false;
};
