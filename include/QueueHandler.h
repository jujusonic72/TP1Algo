#pragma once
#include "HashMap.h"
#include "NodeList.h"
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <functional>

#include "Player.h"

class Command;  // ← Déclaration anticipée

class QueueHandler
{
private:
    typedef NodeList<Command*> CommandList;
        CommandList list;
    enum cmdEnum
    {
        move,
        attack,
        gather,
        use
    };
    typedef std::hash<std::string> hash;
    typedef HashMap<std::string, cmdEnum, hash> CommandMap;
    CommandMap cmdMap;
public:
    QueueHandler() { fillCmdMap();}
    void enqueue(Command* command);
    void nextCommand();
    void removeCommandAtPosition(int index);
    Command* front();
    bool isEmpty();
    int size();
    Player* get_player() const { return player; }
    void set_player(Player* p) { player = p; }
    void fillCmdMap();
    cmdEnum string_to_command(const std::string& commandStr);

    Command* parse_and_validate(const std::string& input);
protected:
    Player* player = nullptr;
};

inline void QueueHandler::fillCmdMap(){
    cmdMap = {
        {"move", move},
        {"attack", attack},
        {"gather", gather},
        {"use", use}
    };
};

inline QueueHandler::cmdEnum QueueHandler::string_to_command(const std::string& commandStr)
{
    auto it = cmdMap.find(commandStr);
    if (!(it == cmdMap.end()))
    {
        auto cmd = *it;
        return cmd.value();
    }
    throw std::invalid_argument("Invalid command string");
};