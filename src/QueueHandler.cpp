#include "QueueHandler.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <vector>

#include "Command.h"
#include "Commands/MoveCommand.h"
#include "Commands/CancelCommand.h"
#include "Commands/WaitCommand.h"
#include "Commands/AttackCommand.h"
#include "Commands/UseCommand.h"
#include "Commands/TalkCommand.h"

void QueueHandler::enqueue(Command* command)
{
    list.insertBack(command);
}

void QueueHandler::nextCommand()
{
    delete front();
    list.eraseFront();
    std::cout << "removed finished command \n";
}

void QueueHandler::removeCommandAtPosition(int position)
{
    if (position > 0 && position <= list.size())
    {
        CommandList::Iterator it = list.begin();
            
        for (int i = 0; i < position-1; ++i) {
            ++it;
        }
        list.erase(it);
    }
    
}

Command* QueueHandler::front()
{
    return *list.begin();
}

bool QueueHandler::isEmpty()
{
    return list.empty();
}

int QueueHandler::size()
{
    return list.size();
}


Command* QueueHandler::parse_and_validate(const std::string& input)
{
    std::vector<std::string> cmdComponents;
    std::stringstream ss(input);
    std::string substr;
    while (ss >> substr)
    {
        cmdComponents.push_back(substr);
    }
    if (cmdComponents.empty())
    {
        std::cerr << "Invalid command string" << "\n";
        return nullptr;
    }
    try
    {

        //cmdEnum cmd = string_to_command(cmdComponents[0]);
        std::string cmd = cmdComponents[0];
        Command* command = nullptr;
        if(cmd == "move")
        {
                Position pos;
                if (cmdComponents.size() == 3)
                {
                    pos = {std::stof(cmdComponents[1]), std::stof(cmdComponents[2])};
                }
                else
                {
                    std::cerr << "Error: Invalid position for move command" << "\n";
                    return nullptr;
                }
                // Créer et retourner un MoveCommand
                command = new MoveCommand(pos);
                std::cout << "Moving to {" << pos.x << ", " << pos.y << "}";
                std::cout << "\n";

                command->set_name(cmdComponents[0] + " " + cmdComponents[1] + " " + cmdComponents[2]);

                return command;
        }
        else if(cmd == "attack")
        {
            // Créer et retourner un AttackCommand

            if(cmdComponents.size() != 1)
            {
                std::cerr << "Error: Attack command takes no arguments" << "\n";
                return nullptr;
            }

            command = new AttackCommand();
            std::cout << "Attacking" << "\n";

            command->set_name(cmdComponents[0]);

            return command;
        }
        else if(cmd == "gather")
        {
            // Créer et retourner un GatherCommand
            //command = new MoveCommand();
            std::cout << "Gathering" << "\n";

            // command->set_name(cmdComponents[0]);
            
            return command;
        }
        else if(cmd == "use")
        {
            if (cmdComponents.size() == 2)
            {
                if(cmdComponents[1].find_first_not_of("0123456789") != std::string::npos)
                {
                    std::cerr << "Error: Invalid inventory slot" << "\n";
                    return nullptr;
                }
                else {
                  int slot = std::stoi(cmdComponents[1]);
                  if(slot < 1 || slot > player->getInventory()->getMaxCapacity())
                  {
                      std::cerr << "Error: Item slot must be between 1 and " << player->getInventory()->getMaxCapacity() << "\n";
                      return nullptr;
                  }

                  command = new UseCommand(slot);
                  std::cout << "Using: " << cmdComponents[1] << "\n";
      
                  command->set_name(cmdComponents[0] + " " + cmdComponents[1]);
  
                  return command;
                }
            }
            else {
              std::cerr << "Error: Invalid item for use command" << "\n";
              return nullptr;
            }
        }
        else if(cmd == "cancel")
        {
            if(size() == 0)
            {
                std::cerr << "Error: No command to cancel" << "\n";
                return nullptr;
            }

            if(cmdComponents.size() != 1 && cmdComponents.size() != 2)
            {
                std::cerr << "Error: Bad arguments for cancel command" << "\n";
                return nullptr;
            }

            if(cmdComponents.size() == 2) {
                if(cmdComponents[1].find_first_not_of("0123456789") != std::string::npos)
                {
                    std::cerr << "Error: Invalid cancel argument" << "\n";
                    return nullptr;
                }
                int position = std::stoi(cmdComponents[1]);
                if(position < 1 || position > size())
                {
                    std::cerr << "Error: Cancel position must be between 1 and " << size() << "\n";
                    return nullptr;
                }
                command = new CancelCommand(position);
                std::cout << "Cancelling current command at position " << position << "\n";
                return command;
            }

            command = new CancelCommand(0);
            std::cout << "Cancelling current command" << "\n";
            return command;
        }
        else if(cmd == "wait")
        {
            if(cmdComponents.size() == 2)
            {
              if(cmdComponents[1].find_first_not_of("0123456789") != std::string::npos)
              {
                  std::cerr << "Error: Invalid wait time" << "\n";
                  return nullptr;
              }
              else {
                  int durationMs = std::stoi(cmdComponents[1]);
                  if(durationMs <= 0)
                  {
                      std::cerr << "Error: Wait time must be greater than 0" << "\n";
                      return nullptr;
                  }

                  command = new WaitCommand(durationMs);
                  std::cout << "Waiting for " << durationMs << " milliseconds" << "\n";

                  command->set_name(cmdComponents[0] + " " + cmdComponents[1] + "ms");

                  return command;
              }
            }
            else {
              std::cerr << "Error: Invalid wait command / too many arguments" << "\n";
              return nullptr;
            }
        }
        else if(cmd == "talk")
        {
            if(cmdComponents.size() != 1)
            {
                std::cerr << "Error: Talk command takes no arguments" << "\n";
                return nullptr;
            }

            command = new TalkCommand();
            std::cout << "Talking to NPC" << "\n";

            command->set_name(cmdComponents[0]);

            return command;
        }
        else if(cmd == "quit" || cmd == "exit")
        {
            std::cout << "Exiting the game." << "\n";
            exit(0);
        }
        return nullptr;
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << e.what() << "\n";
        return nullptr;
    }
    
    
}

void QueueHandler::renderQueue(SDL_Renderer* renderer, TTF_Font* font, int x, int y)
{
    if (isEmpty()) return;

    int line_height = 30;
    int queue_size = size();

    // Limit rendering to at most 7 items
    int displayed = std::min(queue_size, 7);
    int start_y = y - (displayed * line_height);
    int current_y = start_y;
    int index = 1;

    for (auto it = list.begin(); it != list.end(); ++it) {
        if (index > displayed) break;

        Command* cmd = *it;

        std::string display_text = (index == 1)
            ? "(Current) " + cmd->get_name()
            : std::to_string(index) + ". " + cmd->get_name();

        SDL_Color text_color = {200, 200, 200, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, display_text.c_str(),
                                                    display_text.size(), text_color);

        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FRect rect = {
                static_cast<float>(x),
                static_cast<float>(current_y),
                static_cast<float>(surface->w),
                static_cast<float>(surface->h)
            };
            SDL_RenderTexture(renderer, texture, nullptr, &rect);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }

        current_y += line_height;
        index++;
    }
}
