#pragma once
#include "NodeList.h"
#include <string>
#include <stdexcept>
#include <functional>
#include <SDL3_ttf/SDL_ttf.h>

#include "Player.h"

class Command;  // ← Déclaration anticipée

class QueueHandler
{
private:
    typedef NodeList<Command*> CommandList;
        CommandList list;
public:
    QueueHandler() {}
    void enqueue(Command* command);
    void nextCommand();
    void removeCommandAtPosition(int index);
    Command* front();
    bool isEmpty();
    int size();
    Player* get_player() const { return player; }
    void set_player(Player* p) { player = p; }
    void fillCmdMap();

    Command* parse_and_validate(const std::string& input);

    void renderQueue(SDL_Renderer* renderer, TTF_Font* font, int x, int y);
    
    // Pour pouvoir itérer (nécessaire pour renderQueue)
    CommandList::Iterator begin() { return list.begin(); }
    CommandList::Iterator end() { return list.end(); }
protected:
    Player* player = nullptr;
};


