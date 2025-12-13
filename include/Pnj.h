#pragma once
#include "NodeList.h"
#include "Position.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

using namespace std;

class Pnj
{
public:
    Pnj(Position spawn_pos);
    ~Pnj();

    int get_range() const { return range; }
    void set_range(int newRange);

    Position getPosition() const { return position; }
    void SetPosition(Position newPosition);
    
    // Interaction
    void interact();  // Passe au dialogue suivant
    std::string getCurrentDialogue();
    bool isPlayerInRange(Position player_pos) const;

    // Rendu
    bool loadSprite(SDL_Renderer* renderer, const char* filepath, int target_size = 64);
    void render(SDL_Renderer* renderer, Position screen_pos);
    void renderDialogue(SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height);

private:
    int range;
    Position position;
    
    NodeList<string> dialogues;
    int current_dialogue_index = 0;
    
    SDL_Texture* sprite_texture = nullptr;
    int sprite_width = 0;                 
    int sprite_height = 0;                
};