#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Player::Player()
    : health(100), damage(10), attackRange(1), speed(50), position{0, 0}
{
}

Player::~Player()
{
    if (sprite_texture) {
        SDL_DestroyTexture(sprite_texture);
    }
}

bool Player::loadSprite(SDL_Renderer* renderer, const char* filepath, int target_size)
{
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cout << "Failed to load sprite: " << SDL_GetError() << "\n";
        return false;
    }
    
    std::cout << "Original size: " << surface->w << "x" << surface->h << "\n";
    std::cout << "Target size: " << target_size << "%\n";
    
    // Redimensionner à la taille cible
    sprite_width = (int)(float(surface->w) * (target_size / 100.0f));
    sprite_height = (int)(float(surface->h) * (target_size / 100.0f));
    
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

void Player::SetHealth(int newHealth)
{
    health = newHealth;
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