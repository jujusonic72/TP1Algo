#include "QueueHandler.h"
#include <sstream>
#include <array>
#include <iostream>
#include <vector>

#include "Command.h"
#include "../include/Commands/MoveCommand.h"

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
        cmdEnum cmd = string_to_command(cmdComponents[0]);
        Command* command = nullptr;
        switch (cmd)
        {
        case move:
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
        case attack:
            // Créer et retourner un AttackCommand
            //command = new MoveCommand();
            std::cout << "Attacking" << "\n";
            return command;
        case gather:
            // Créer et retourner un GatherCommand
            //command = new MoveCommand();
            std::cout << "Gathering" << "\n";
            return command;
        case use:
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
        std::cerr << "Error: Invalid command passed" << "\n";
        return nullptr;
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << e.what() << "\n";
        return nullptr;
    }
    
    
}
