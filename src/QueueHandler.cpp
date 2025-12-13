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
    NodeList<std::string> cmdComponents;  // NodeList au lieu de vector
    std::stringstream ss(input);
    std::string substr;
    while (ss >> substr)
    {
        cmdComponents.insertBack(substr);  // insertBack au lieu de push_back
    }
    
    if (cmdComponents.empty())
    {
        std::cerr << "Invalid command string" << "\n";
        return nullptr;
    }
    
    try
    {
        // Récupérer le premier élément
        auto it = cmdComponents.begin();
        std::string cmd = *it;
        
        Command* command = nullptr;
        
        if(cmd == "move")
        {
            Position pos;
            if (cmdComponents.size() == 3)
            {
                ++it;
                std::string arg1 = *it;
                ++it;
                std::string arg2 = *it;
                pos = {std::stof(arg1), std::stof(arg2)};
            }
            else
            {
                std::cerr << "Error: Invalid position for move command" << "\n";
                return nullptr;
            }
            
            command = new MoveCommand(pos);
            std::cout << "Moving to {" << pos.x << ", " << pos.y << "}\n";
            
            // Reconstruire le nom
            it = cmdComponents.begin();
            std::string name = *it;
            ++it;
            name += " " + *it;
            ++it;
            name += " " + *it;
            command->set_name(name);
            
            return command;
        }
        else if(cmd == "attack")
        {
            if(cmdComponents.size() != 1)
            {
                std::cerr << "Error: Attack command takes no arguments" << "\n";
                return nullptr;
            }

            command = new AttackCommand();
            std::cout << "Attacking\n";
            command->set_name(cmd);
            return command;
        }
        else if(cmd == "gather")
        {
            std::cout << "Gathering\n";
            return command;
        }
        else if(cmd == "use")
        {
            if (cmdComponents.size() == 2)
            {
                ++it;
                std::string slotStr = *it;
                
                if(slotStr.find_first_not_of("0123456789") != std::string::npos)
                {
                    std::cerr << "Error: Invalid inventory slot\n";
                    return nullptr;
                }
                
                int slot = std::stoi(slotStr);
                if(slot < 1 || slot > player->getInventory()->getMaxCapacity())
                {
                    std::cerr << "Error: Item slot must be between 1 and " 
                              << player->getInventory()->getMaxCapacity() << "\n";
                    return nullptr;
                }

                command = new UseCommand(slot);
                std::cout << "Using: " << slotStr << "\n";
                
                it = cmdComponents.begin();
                command->set_name(*it + " " + slotStr);
                
                return command;
            }
            else
            {
                std::cerr << "Error: Invalid item for use command\n";
                return nullptr;
            }
        }
        else if(cmd == "cancel")
        {
            if(size() == 0)
            {
                std::cerr << "Error: No command to cancel\n";
                return nullptr;
            }

            if(cmdComponents.size() != 1 && cmdComponents.size() != 2)
            {
                std::cerr << "Error: Bad arguments for cancel command\n";
                return nullptr;
            }

            if(cmdComponents.size() == 2)
            {
                ++it;
                std::string posStr = *it;
                
                if(posStr.find_first_not_of("0123456789") != std::string::npos)
                {
                    std::cerr << "Error: Invalid cancel argument\n";
                    return nullptr;
                }
                
                int position = std::stoi(posStr);
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
            std::cout << "Cancelling current command\n";
            return command;
        }
        else if(cmd == "wait")
        {
            if(cmdComponents.size() == 2)
            {
                ++it;
                std::string durationStr = *it;
                
                if(durationStr.find_first_not_of("0123456789") != std::string::npos)
                {
                    std::cerr << "Error: Invalid wait time\n";
                    return nullptr;
                }
                
                int durationMs = std::stoi(durationStr);
                if(durationMs <= 0)
                {
                    std::cerr << "Error: Wait time must be greater than 0\n";
                    return nullptr;
                }

                command = new WaitCommand(durationMs);
                std::cout << "Waiting for " << durationMs << " milliseconds\n";
                
                it = cmdComponents.begin();
                command->set_name(*it + " " + durationStr + "ms");
                
                return command;
            }
            else
            {
                std::cerr << "Error: Invalid wait command / too many arguments\n";
                return nullptr;
            }
        }
        else if(cmd == "talk")
        {
            if(cmdComponents.size() != 1)
            {
                std::cerr << "Error: Talk command takes no arguments\n";
                return nullptr;
            }

            command = new TalkCommand();
            std::cout << "Talking to NPC\n";
            command->set_name(cmd);
            return command;
        }
        else if(cmd == "quit" || cmd == "exit")
        {
            std::cout << "Exiting the game.\n";
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
