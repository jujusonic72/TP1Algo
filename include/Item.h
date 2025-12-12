#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "Position.h"

enum class ItemType
{
    HEALTH_POTION,
    DAMAGE_BOOST,
    SPEED_BOOST,
    TREASURE,
    WEAPON,
    KEY
};

class Item
{
public:
    Item();
    Item(ItemType type, const std::string& name, const std::string& description, int value);
    ~Item();

    // Getters
    ItemType getType() const { return type_; }
    std::string getName() const { return name_; }
    std::string getDescription() const { return description_; }
    int getValue() const { return value_; }
    Position getPosition() const { return position_; }
    bool isOnGround() const { return on_ground_; }

    // Setters
    void setPosition(Position pos) { position_ = pos; }
    void setOnGround(bool on_ground) { on_ground_ = on_ground; }

    // Méthodes pour le rendu
    bool loadSprite(SDL_Renderer* renderer, const char* filepath);
    void render(SDL_Renderer* renderer, Position screen_pos);

    // Méthode statique pour créer des items prédéfinis
    static Item* createHealthPotion();
    static Item* createDamageBoost();
    static Item* createSpeedBoost();
    static Item* createTreasure();

protected:
    ItemType type_;
    std::string name_;
    std::string description_;
    int value_;  // Valeur de l'effet (ex: +50 HP, +10 damage, etc.)
    Position position_;
    bool on_ground_;  // Est-ce que l'item est au sol ou dans l'inventaire?

    SDL_Texture* sprite_texture_ = nullptr;
    int sprite_width_ = 0;
    int sprite_height_ = 0;
};
