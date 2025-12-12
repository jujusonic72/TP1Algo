#include "Camera.h"

Camera::Camera(int window_width, int window_height, Player* player)
    : player_ref(player), window_height(window_height), window_width(window_width)
{
    
}

Camera::~Camera()
{
    // Rien à nettoyer pour l'instant
}