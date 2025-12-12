#pragma once
#include "Position.h"
#include <SDL3/SDL.h>

class Player
{
public:
    Player();
    ~Player();

    int getHealth() const {return health;}
    void SetHealth(int newHealth);
    
    int getDamage() const {return damage;}
    void SetDamage(int newDamage);

    int getAttackRange() const {return attackRange;}
    void SetAttackRange(int newAttackRange);
    
    int getSpeed() const {return speed;}
    void SetSpeed(int newSpeed);
    
    Position getPosition() const {return position;}
    void SetPosition(Position newPosition);

    // méthodes pour le rendu
    bool loadSprite(SDL_Renderer* renderer, const char* filepath, float target_size = 64);
    void render(SDL_Renderer* renderer, Position screen_pos);

protected:
    int health;
    int damage;
    int attackRange;
    int speed;
    Position position;
    
    SDL_Texture* sprite_texture = nullptr;
    int sprite_width = 0;                 
    int sprite_height = 0;                
};