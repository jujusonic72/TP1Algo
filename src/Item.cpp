#include "Item.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Item::Item() 
    : type_(ItemType::HEALTH_POTION), 
      name_("Item"), 
      description_("An item"), 
      value_(0),
      position_{0, 0},
      on_ground_(false)
{
}

Item::Item(ItemType type, const std::string& name, const std::string& description, int value)
    : type_(type),
      name_(name),
      description_(description),
      value_(value),
      position_{0, 0},
      on_ground_(false)
{
}

Item::~Item()
{
    if (sprite_texture_) {
        SDL_DestroyTexture(sprite_texture_);
        sprite_texture_ = nullptr;
    }
}

bool Item::loadSprite(SDL_Renderer* renderer, const char* filepath)
{
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cout << "Failed to load item sprite: " << filepath << " - " << SDL_GetError() << "\n";
        return false;
    }

    sprite_texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    sprite_width_ = surface->w;
    sprite_height_ = surface->h;
    SDL_DestroySurface(surface);

    if (!sprite_texture_) {
        std::cout << "Failed to create texture from surface: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

void Item::render(SDL_Renderer* renderer, Position screen_pos)
{
    if (!sprite_texture_) {
        // Dessiner un rectangle coloré selon le type si pas de sprite
        SDL_FRect rect = {
            static_cast<float>(screen_pos.x - 10),
            static_cast<float>(screen_pos.y - 10),
            20.0f,
            20.0f
        };

        // Couleur selon le type
        switch (type_) {
            case ItemType::HEALTH_POTION:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
                break;
            case ItemType::DAMAGE_BOOST:
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange
                break;
            case ItemType::SPEED_BOOST:
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan
                break;
            
        }
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_FRect dest = {
            static_cast<float>(screen_pos.x - sprite_width_ / 2),
            static_cast<float>(screen_pos.y - sprite_height_ / 2),
            static_cast<float>(sprite_width_),
            static_cast<float>(sprite_height_)
        };
        SDL_RenderTexture(renderer, sprite_texture_, nullptr, &dest);
    }
}

// Factory methods pour créer des items prédéfinis
Item* Item::createHealthPotion()
{
    return new Item(ItemType::HEALTH_POTION, "Health Potion", "Restores 50 HP", 50);
}

Item* Item::createDamageBoost()
{
    return new Item(ItemType::DAMAGE_BOOST, "Damage Boost", "Increases damage by 10 for 30 seconds", 10);
}

Item* Item::createSpeedBoost()
{
    return new Item(ItemType::SPEED_BOOST, "Speed Boost", "Increases movement speed by 20 for 30 seconds", 20);
}

