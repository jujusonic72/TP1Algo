#pragma once
#include "NodeList.h"
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "Player.h"

class Command;  // ← Déclaration anticipée! Tu dis juste "Command existe"

class QueueHandler
{
private:
    typedef NodeList<Command*> CommandList;
        CommandList list;
public:
    void enqueue(Command* command);
    void nextCommand();
    void removeCommandAtPosition(int index);
    Command* front();
    bool isEmpty();
    int size();
    Player* get_player() const { return player; }
    void set_player(Player* p) { player = p; }
    enum cmdEnum
    {
        move,
        attack,
        gather,
        use
    };
    std::unordered_map<std::string, cmdEnum> cmdMap = {
        {"move", move},
        {"attack", attack},
        {"gather", gather},
        {"use", use}
    };
    cmdEnum string_to_command(const std::string& commandStr)
    {
        auto it = cmdMap.find(commandStr);
        if (it != cmdMap.end())
        {
            return it->second;
        }
        throw std::invalid_argument("Invalid command string");
    }

    Command* parse_and_validate(const std::string& input);
protected:
    Player* player = nullptr;
};
