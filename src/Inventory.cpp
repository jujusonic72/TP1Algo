#include "Inventory.h"
#include <iostream>

Inventory::Inventory() : max_capacity_(20)
{
}

Inventory::Inventory(int maxCapacity) : max_capacity_(maxCapacity)
{
}

Inventory::~Inventory()
{
    // Nettoyer tous les items
    while (!items_.empty()) {
        Item* item = *items_.begin();
        delete item;
        items_.eraseFront();
    }
}

bool Inventory::addItem(Item* item)
{
    if (isFull()) {
        std::cout << "Inventory is full! Cannot add " << item->getName() << "\n";
        return false;
    }

    items_.insertBack(item);
    item->setOnGround(false);
    std::cout << "Added " << item->getName() << " to inventory\n";
    return true;
}

bool Inventory::removeItem(Item* item)
{
    auto it = items_.begin();
    while (it != items_.end()) {
        if (*it == item) {
            items_.erase(it);
            std::cout << "Removed " << item->getName() << " from inventory\n";
            return true;
        }
        ++it;
    }
    return false;
}

Item* Inventory::removeItemAt(int index)
{
    if (index < 0 || index >= items_.size()) {
        return nullptr;
    }

    auto it = items_.begin();
    for (int i = 0; i < index; ++i) {
        ++it;
    }

    Item* item = *it;
    items_.erase(it);
    std::cout << "Removed " << item->getName() << " from inventory\n";
    return item;
}

Item* Inventory::getItemAt(int index)
{
    if (index < 0 || index >= items_.size()) {
        return nullptr;
    }

    auto it = items_.begin();
    for (int i = 0; i < index; ++i) {
        ++it;
    }

    return *it;
}

Item* Inventory::findItemByType(ItemType type)
{
    auto it = items_.begin();
    while (it != items_.end()) {
        if ((*it)->getType() == type) {
            return *it;
        }
        ++it;
    }
    return nullptr;
}

void Inventory::render(SDL_Renderer* renderer, int x, int y, int width, int height)
{
    // Dessiner le fond de l'inventaire
    SDL_FRect bg_rect = {
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(width),
        static_cast<float>(height)
    };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
    SDL_RenderFillRect(renderer, &bg_rect);

    // Bordure
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderRect(renderer, &bg_rect);

    // Afficher le titre "INVENTORY"
    // (Pour l'instant sans texte, on ajoutera TTF plus tard si nécessaire)

    // Calculer la disposition des items (grille 4x5)
    int cols = 4;
    int item_size = 40;
    int padding = 10;
    int start_x = x + padding;
    int start_y = y + padding + 20; // Espace pour le titre

    // Dessiner les slots
    for (int i = 0; i < max_capacity_; ++i) {
        int col = i % cols;
        int row = i / cols;
        int slot_x = start_x + col * (item_size + padding);
        int slot_y = start_y + row * (item_size + padding);

        // Dessiner le slot
        SDL_FRect slot_rect = {
            static_cast<float>(slot_x),
            static_cast<float>(slot_y),
            static_cast<float>(item_size),
            static_cast<float>(item_size)
        };
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderFillRect(renderer, &slot_rect);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderRect(renderer, &slot_rect);
    }

    // Dessiner les items dans les slots
    int index = 0;
    auto it = items_.begin();
    while (it != items_.end() && index < max_capacity_) {
        Item* item = *it;
        
        int col = index % cols;
        int row = index / cols;
        int slot_x = start_x + col * (item_size + padding) + item_size / 2;
        int slot_y = start_y + row * (item_size + padding) + item_size / 2;

        // Dessiner l'item (version simplifiée sans sprite)
        SDL_FRect item_rect = {
            static_cast<float>(slot_x - 20),
            static_cast<float>(slot_y - 20),
            30.0f,
            30.0f
        };

        // Couleur selon le type
        switch (item->getType()) {
            case ItemType::HEALTH_POTION:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            case ItemType::DAMAGE_BOOST:
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                break;
            case ItemType::SPEED_BOOST:
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                break;
           
        }
        SDL_RenderFillRect(renderer, &item_rect);

        ++it;
        ++index;
    }
}
