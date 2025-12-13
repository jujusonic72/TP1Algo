#pragma once
#include "Position.h"
#include "Inventory.h"
#include <SDL3/SDL.h>

class Player
{
public:
    Player();
    ~Player();

    int getHealth() const {return health;}
    void SetHealth(int newHealth);
    
    int getMaxHealth() const {return max_health;}
    void SetMaxHealth(int newMaxHealth);
    
    int getDamage() const {return damage;}
    void SetDamage(int newDamage);

    int getAttackRange() const {return attackRange;}
    void SetAttackRange(int newAttackRange);
    
    int getSpeed() const {return speed;}
    void SetSpeed(int newSpeed);
    
    Position getPosition() const {return position;}
    void SetPosition(Position newPosition);

    // Gestion de l'inventaire
    Inventory* getInventory() { return inventory; }
    bool pickupItem(Item* item);
    bool useItem(int index);

    void update(Uint64 delta_time);

    void applySpeedBoost(int boost_value, Uint64 duration_ms);
    void applyDamageBoost(int boost_value, Uint64 duration_ms);
    int getEffectiveSpeed() const;
    int getEffectiveDamage() const;

    // méthodes pour le rendu
    bool loadSprite(SDL_Renderer* renderer, const char* filepath, float target_size = 64);
    void render(SDL_Renderer* renderer, Position screen_pos);
    void renderUI(SDL_Renderer* renderer, int window_width, int window_height);

protected:
    int health;
    int max_health;
    int damage;
    int attackRange;
    int speed;
    Position position;

    int speed_boost = 0;
    Uint64 speed_boost_end = 0;
    
    int damage_boost = 0;
    Uint64 damage_boost_end = 0;
    
    Inventory* inventory;
    
    SDL_Texture* sprite_texture = nullptr;
    int sprite_width = 0;                 
    int sprite_height = 0;                
};
