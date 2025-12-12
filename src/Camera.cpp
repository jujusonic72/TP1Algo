#include "Camera.h"

Camera::Camera(int window_width, int window_height, Player* player)
    : player_ref(player)
{
    center_screen.x = window_width / 2;
    center_screen.y = window_height / 2;
}

Camera::~Camera()
{
    // Rien à nettoyer pour l'instant
}