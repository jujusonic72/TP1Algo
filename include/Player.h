#pragma once
#include "Position.h"

class Player
{
public:
    Player();
    ~Player();

    // Getters and Setters
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


    // Inventory
    
protected:
    int health;
    int damage;
    int attackRange;
    int speed;
    Position position;
};
