#include "Player.h"

Player::Player()
    : health(100), damage(10), attackRange(1), speed(50), position{0, 0}
{
}

Player::~Player()
{
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
