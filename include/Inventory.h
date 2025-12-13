#pragma once
#include "NodeList.h"
#include "Item.h"
#include <SDL3/SDL.h>

class Inventory
{
public:
    Inventory();
    Inventory(int maxCapacity);
    ~Inventory();

    // Gestion des items
    bool addItem(Item* item);
    bool removeItem(Item* item);
    Item* removeItemAt(int index);
    Item* getItemAt(int index);
    Item* findItemByType(ItemType type);
    
    // Informations
    int getSize() const { return items_.size(); }
    int getMaxCapacity() const { return max_capacity_; }
    bool isFull() const { return items_.size() >= max_capacity_; }
    bool isEmpty() const { return items_.empty(); }
    
    // Itération
    NodeList<Item*>::Iterator begin() { return items_.begin(); }
    NodeList<Item*>::Iterator end() { return items_.end(); }

    // Rendu
    void render(SDL_Renderer* renderer, int x, int y, int width, int height);

private:
    NodeList<Item*> items_;
    int max_capacity_;
};
