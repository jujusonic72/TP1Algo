#pragma once
#include "NodeList.h"
#include "Player.h"
#include "Position.h"
#include <iostream>
#include <stdexcept>
#include <SDL3/SDL.h>
#include <type_traits>

using namespace std;
class Camera
{
public:
    Camera(int window_width, int window_height, Player* player);
    ~Camera(); // Destructeur
    Player* player_ref;

    Position player_pos() {return player_ref->getPosition();}
    void get_center_screen(int height, int width) 
    {
        center_screen.x = width / 2;
        center_screen.y = height / 2;
    }
    Position pos_to_screen(Position world_pos, int height, int width) { 
        Position screen_pos;
        get_center_screen(height, width);
        screen_pos.x = center_screen.x + (world_pos.x - player_pos().x);
        screen_pos.y = center_screen.y + (world_pos.y - player_pos().y);
        return screen_pos;
    }

    
    
private:
    Position center_screen;
    int window_height, window_width;
};