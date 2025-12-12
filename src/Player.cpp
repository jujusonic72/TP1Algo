#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Player::Player()
    : health(100), max_health(100), damage(10), attackRange(100), speed(50), position{0, 0}
{
    inventory = new Inventory(20); // Inventaire de 20 slots
}

Player::~Player()
{
    if (sprite_texture) {
        SDL_DestroyTexture(sprite_texture);
    }
    delete inventory;
}

bool Player::loadSprite(SDL_Renderer* renderer, const char* filepath, float target_size)
{
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cout << "Failed to load sprite: " << SDL_GetError() << "\n";
        return false;
    }
    
    std::cout << "Original size: " << surface->w << "x" << surface->h << "\n";
    std::cout << "Target size: " << target_size << "%\n";
    
    // Redimensionner à la taille cible
    float og_width = surface->w;
    float og_height = surface->h;
    sprite_width = (int)(og_width * (target_size / 100.0f));
    sprite_height = (int)(og_height * (target_size / 100.0f));
    
    std::cout << "Final sprite size: " << sprite_width << "x" << sprite_height << "\n";
    
    sprite_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!sprite_texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << "\n";
        return false;
    }
    
    std::cout << "Sprite loaded successfully: " << filepath << "\n";
    return true;
}

void Player::render(SDL_Renderer* renderer, Position screen_pos)
{
    if (!sprite_texture) {
        std::cout << "No sprite texture loaded!\n";
        return;
    }
    //std::cout << "Rendering player at screen: (" << screen_pos.x << ", " << screen_pos.y << ")\n";
    SDL_FRect dest = {
        static_cast<float>(screen_pos.x - sprite_width/2),
        static_cast<float>(screen_pos.y - sprite_height/2),
        static_cast<float>(sprite_width),
        static_cast<float>(sprite_height)
    };
    
    SDL_RenderTexture(renderer, sprite_texture, nullptr, &dest);
}

void Player::renderUI(SDL_Renderer* renderer, int window_width, int window_height)
{
    // Afficher la barre de vie
    int health_bar_width = 200;
    int health_bar_height = 20;
    int health_bar_x = 10;
    int health_bar_y = 10;
    
    // Fond de la barre
    SDL_FRect bg_rect = {
        static_cast<float>(health_bar_x),
        static_cast<float>(health_bar_y),
        static_cast<float>(health_bar_width),
        static_cast<float>(health_bar_height)
    };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &bg_rect);
    
    // Barre de vie actuelle
    float health_percentage = static_cast<float>(health) / max_health;
    SDL_FRect health_rect = {
        static_cast<float>(health_bar_x),
        static_cast<float>(health_bar_y),
        static_cast<float>(health_bar_width * health_percentage),
        static_cast<float>(health_bar_height)
    };
    
    // Couleur selon le pourcentage de vie
    if (health_percentage > 0.6f) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
    } else if (health_percentage > 0.3f) {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
    }
    SDL_RenderFillRect(renderer, &health_rect);
    
    // Bordure
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bg_rect);
    
    // Afficher l'inventaire (dans le coin inférieur droit)
    int inv_width = 220;
    int inv_height = 280;
    int inv_x = window_width - inv_width - 10;
    int inv_y = window_height - inv_height - 10;
    
    inventory->render(renderer, inv_x, inv_y, inv_width, inv_height);
}

bool Player::pickupItem(Item* item)
{
    return inventory->addItem(item);
}

bool Player::useItem(int index)
{
    Item* item = inventory->getItemAt(index);
    if (!item) {
        return false;
    }
    
    // Appliquer l'effet de l'item
    switch (item->getType()) {
        case ItemType::HEALTH_POTION:
            health = (health + item->getValue() > max_health) ? max_health : health + item->getValue();
            std::cout << "Used Health Potion! HP: " << health << "/" << max_health << "\n";
            break;
            
        case ItemType::DAMAGE_BOOST:
            damage += item->getValue();
            std::cout << "Used Damage Boost! Damage: " << damage << "\n";
            break;
            
        case ItemType::SPEED_BOOST:
            speed += item->getValue();
            std::cout << "Used Speed Boost! Speed: " << speed << "\n";
            break;
            
        case ItemType::TREASURE:
            std::cout << "Found treasure worth " << item->getValue() << " gold!\n";
            break;
            
        default:
            std::cout << "Cannot use this item\n";
            return false;
    }
    
    // Retirer l'item de l'inventaire après utilisation
    delete inventory->removeItemAt(index);
    return true;
}

void Player::SetHealth(int newHealth)
{
    health = newHealth;
    if (health > max_health) {
        health = max_health;
    }
    if (health < 0) {
        health = 0;
    }
}

void Player::SetMaxHealth(int newMaxHealth)
{
    max_health = newMaxHealth;
}

void Player::SetDamage(int newDamage)
{
    damage = newDamage;
}

void Player::SetAttackRange(int newAttackRange)
{
    attackRange = newAttackRange;
}

void Player::SetSpeed(int newSpeed)
{
    speed = newSpeed;
}

void Player::SetPosition(Position newPosition)
{
    position = newPosition;
}
