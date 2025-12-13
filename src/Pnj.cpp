#include "Pnj.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cmath>

Pnj::Pnj(Position spawn_pos) 
    : range(100), position(spawn_pos), current_dialogue_index(0)
{
    dialogues.insertBack("Bonjour aventurier !");
    dialogues.insertBack("C'est une merveilleuse journée pour pêcher n'est-ce pas?");
    dialogues.insertBack("hu-huh!");
    dialogues.insertBack("Bonne chance aventurier !");
}

Pnj::~Pnj()
{
    if (sprite_texture) {
        SDL_DestroyTexture(sprite_texture);
    }
}

void Pnj::set_range(int newRange)
{
    range = newRange;
}

void Pnj::SetPosition(Position newPosition)
{
    position = newPosition;
}

void Pnj::interact()
{
    current_dialogue_index++;
    if (current_dialogue_index >= dialogues.size()) {
        current_dialogue_index = 0;  // Recommencer du début
    }
}

std::string Pnj::getCurrentDialogue()
{
    if (dialogues.empty()) return "";
    
    // Parcourir jusqu'à l'index
    auto it = dialogues.begin();
    for (int i = 0; i < current_dialogue_index && it != dialogues.end(); ++i) {
        ++it;
    }
    
    if (it != dialogues.end()) {
        return *it;
    }
    return "";
}

bool Pnj::isPlayerInRange(Position player_pos) const
{
    float dx = player_pos.x - position.x;
    float dy = player_pos.y - position.y;
    float distance = sqrt(dx*dx + dy*dy);
    return distance <= range;
}

bool Pnj::loadSprite(SDL_Renderer* renderer, const char* filepath, int target_size)
{
    SDL_Surface* surface = IMG_Load(filepath);
    if (!surface) {
        std::cout << "Failed to load PNJ sprite: " << SDL_GetError() << "\n";
        return false;
    }
    
    sprite_width = (int)(surface->w * (target_size / 100.0f));
    sprite_height = (int)(surface->h * (target_size / 100.0f));
    
    sprite_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!sprite_texture) {
        std::cout << "Failed to create PNJ texture: " << SDL_GetError() << "\n";
        return false;
    }
    
    return true;
}

void Pnj::render(SDL_Renderer* renderer, Position screen_pos)
{
    if (!sprite_texture) return;
    
    SDL_FRect dest = {
        static_cast<float>(screen_pos.x - sprite_width/2),
        static_cast<float>(screen_pos.y - sprite_height/2),
        static_cast<float>(sprite_width),
        static_cast<float>(sprite_height)
    };
    
    SDL_RenderTexture(renderer, sprite_texture, nullptr, &dest);
}

void Pnj::renderDialogue(SDL_Renderer* renderer, TTF_Font* font, int window_width, int window_height)
{
    std::string dialogue = getCurrentDialogue();
    if (dialogue.empty()) return;
    
    SDL_Color text_color = {255, 255, 100, 255};  // Jaune
    SDL_Surface* surface = TTF_RenderText_Solid(font, dialogue.c_str(), dialogue.size(), text_color);
    
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        // Centrer le texte en haut de l'écran
        SDL_FRect rect = {
            (float)(window_width / 2 - surface->w / 2),
            50.0f,
            (float)surface->w,
            (float)surface->h
        };
        
        // Fond semi-transparent
        SDL_FRect bg_rect = {rect.x - 10, rect.y - 5, rect.w + 20, rect.h + 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &bg_rect);
        
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
        
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }
}