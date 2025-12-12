#include "QueueHandler.h"
#include <sstream>
#include <iostream>
#include <vector>

#include "Command.h"
#include "../include/Commands/MoveCommand.h"
#include "../include/Commands/CancelCommand.h"

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
                return command;
        }
        else if(cmd == "attack")
        {
            // Créer et retourner un AttackCommand
            //command = new MoveCommand();
            std::cout << "Attacking" << "\n";
            return command;
        }
        else if(cmd == "gather")
        {
            // Créer et retourner un GatherCommand
            //command = new MoveCommand();
            std::cout << "Gathering" << "\n";
            return command;
        }
        else if(cmd == "use")
        {
            if (cmdComponents.size() != 2)
            {
                std::cerr << "Error: Invalid item for use command" << "\n";
                return nullptr;
            }
            // Créer et retourner un UseCommand
            //command = new MoveCommand();
            std::cout << "Using: " << cmdComponents[1];
            std::cout << "\n";
            return command;
        }
        else if(cmd == "cancel")
        {
            if(size() == 0)
            {
                std::cerr << "Error: No command to cancel" << "\n";
                return nullptr;
            }
            command = new CancelCommand();
            std::cout << "Cancelling current command" << "\n";
            return command;
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
    int current_y = y;
    int index = 1;
    
    for (auto it = list.begin(); it != list.end(); ++it) {
        Command* cmd = *it;
        std::string display_text = std::to_string(index) + ". " + cmd->get_name();
        
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
