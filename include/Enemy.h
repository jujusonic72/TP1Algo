#pragma once

class Player;

#include "Position.h"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

class Enemy
{
public:
    Enemy(int health, int damage, int cooldown_time, int attack_range, Position spawn_pos)
        : hp(health), dmg(damage), spd(cooldown_time), rng(attack_range), world_pos(spawn_pos) {
    }

    int getHealth() const { return hp; }
    void SetHealth(int newHealth) { hp = newHealth; }
    int getDamage() const { return dmg; }
    void SetDamage(int newDamage) { dmg = newDamage; }
    int getAttackRange() const { return rng; }
    void SetAttackRange(int newAttackRange) { rng = newAttackRange; }
    int getSpeed() const { return spd; }
    void SetSpeed(int newSpeed) { spd = newSpeed; }  // FIXÉ: était hp = newSpeed
    Position getPosition() const { return world_pos; }
    void SetPosition(Position newPosition) { world_pos = newPosition; }

    bool loadSprite(SDL_Renderer* renderer, float target_size = 64)
    {
        SDL_Surface* surface = IMG_Load(sprite_path);
        if (!surface) {
            std::cout << "Failed to load sprite: " << SDL_GetError() << "\n";
            return false;
        }

        float og_width = surface->w;
        float og_height = surface->h;
        sprite_width = (int)(og_width * (target_size / 100.0f));
        sprite_height = (int)(og_height * (target_size / 100.0f));

        sprite_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (!sprite_texture) {
            std::cout << "Failed to create texture: " << SDL_GetError() << "\n";
            return false;
        }
        return true;
    }

    void render(SDL_Renderer* renderer, Position screen_pos)
    {
        if (!sprite_texture) {
            std::cout << "No sprite texture loaded!\n";
            return;
        }
        SDL_FRect dest = {
            static_cast<float>(screen_pos.x - sprite_width / 2),
            static_cast<float>(screen_pos.y - sprite_height / 2),
            static_cast<float>(sprite_width),
            static_cast<float>(sprite_height)
        };
        SDL_RenderTexture(renderer, sprite_texture, nullptr, &dest);
    }

    void update(Uint64 delta_time, Player* player);  // Déclaration

private:
    Uint64 last_attack_time = 0;

protected:
    Position world_pos;
    int hp, dmg, spd, rng;
    SDL_Texture* sprite_texture = nullptr;
    const char* sprite_path = "./assets/sprites/Enemy.png";
    int sprite_width = 0;
    int sprite_height = 0;
};

// Include Player.h APRÈS la définition de Enemy
#include "Player.h"

// Implémentation inline de update
inline void Enemy::update(Uint64 delta_time, Player* player)
{
    Position player_pos = player->getPosition();
    float dx = player_pos.x - world_pos.x;
    float dy = player_pos.y - world_pos.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance <= rng) {
        Uint64 current_time = SDL_GetTicks();
        if (current_time - last_attack_time >= spd) {
            player->SetHealth(player->getHealth() - dmg);
            last_attack_time = current_time;
            std::cout << "Enemy attacked! Player HP: " << player->getHealth() << "\n";
        }
    }
}
