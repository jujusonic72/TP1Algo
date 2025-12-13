#include "../../include/Commands/UseCommand.h"
#include "Enemy.h"

#include <iostream>

void UseCommand::pre_execute(QueueHandler* queueHandler)
{
    this->queueHandler = queueHandler;
    this->player = queueHandler->get_player();

    queueHandler->enqueue(this);
}

void UseCommand::execute(Uint64 delta_time)
{
    if (force_finish_) {
        post_execute();
        return;
    }
    
    elapsedUseTime += delta_time;
    if (elapsedUseTime >= useTimeMs) {
        trigger_use();

        set_finished(true);
    }

    if (finished_)
    {
        post_execute();
    }
}

void UseCommand::post_execute()
{
    std::cout << "Done waiting\n";
    queueHandler->nextCommand();
}

void UseCommand::trigger_use()
{
  Inventory& inventory = *player->getInventory();
  if(inventory.isEmpty())
  {
      std::cout << "Inventory is empty, cannot use item.\n";
      return;
  }

  if(!player->useItem(itemSlotIndex-1)) {
      std::cout << "No item found in slot " << itemSlotIndex << ".\n";
      return;
  }
}