#pragma once
#include <SDL3/SDL_stdinc.h>
#include <string.h>
#include <string>
#include "QueueHandler.h"
using namespace std;
class Command
{
public:
    Command();
    virtual ~Command();
    virtual void pre_execute(QueueHandler* queueHandler) = 0;
    virtual void execute(Uint64 delta_time) = 0;
    string get_name() {return name;}
    virtual void post_execute();
    bool is_edit_command() const { return is_edit_command_; }
    
protected:
    void set_force_finish(bool b) { force_finish_ = b; }
    void set_finished(bool b) { finished_ = b; }
    QueueHandler* queueHandler = nullptr;
    bool force_finish_ = false, finished_ = false, is_edit_command_ = false;
    string name = "Commande";
};
