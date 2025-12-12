#pragma once
#include "Player.h"
#include "Position.h"
#include <iostream>
#include <stdexcept>
#include <SDL3/SDL.h>

using namespace std;
class Camera
{
public:
    Camera(int window_width, int window_height, Player* player);
    ~Camera(); // Destructeur
    Player* player_ref;
    

    
    
    
    
    
    
    // Quelles méthodes ?
    Position player_pos() {return player_ref->getPosition();}
    Position pos_to_screen(Position world_pos) { 
        Position screen_pos;
        screen_pos.x = center_screen.x + (world_pos.x - player_pos().x);
        screen_pos.y = center_screen.y + (world_pos.y - player_pos().y);
        return screen_pos;
     }
    
private:
    Position center_screen;
    // Quelles variables membres ?
};