#pragma once

#include <iostream>
#include <stdexcept>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>

#include "Command.h"
#include "NodeList.h"
#include "Position.h"
#include "QueueHandler.h"
using namespace std;
class TextBox
{
public:
    TextBox();
    TextBox(QueueHandler* q);
    ~TextBox();
    void handle_event(const SDL_Event& e);
    void render(SDL_Renderer* renderer);
private:
    string text_;
    NodeList<string>* history_;
    TTF_Font* font_;
    Position box_pos_{0, 0};
    QueueHandler* queue_handler_;
    bool cursor_visible_ = true;
    Uint64 last_cursor_toggle_time_ = 0;
    int font_size_, height_, width_, cursor_pos_, history_index_ = 0;
};
inline TextBox::TextBox() : font_size_(12), height_(30), width_(400), cursor_pos_(0)
{
    if (!(font_ = TTF_OpenFont("./assets/fonts/Gugi/Gugi-Regular.ttf", 24))) throw std::runtime_error("Failed to load font");
}
inline TextBox::TextBox(QueueHandler* q) : queue_handler_(q), font_size_(12), height_(30), width_(400), cursor_pos_(0)
{
    history_ = new NodeList<string>();
    if (!(font_ = TTF_OpenFont("./assets/fonts/Gugi/Gugi-Regular.ttf", 24))) throw std::runtime_error("Failed to load font");
}
inline TextBox::~TextBox()
{
    TTF_CloseFont(font_);
}
inline void TextBox::handle_event(const SDL_Event& e)
{
    if (e.type == SDL_EVENT_TEXT_INPUT)
    {
        // TODO: Ajouter le caractère à text_
        text_.insert(cursor_pos_, e.text.text);
        ++cursor_pos_;
    }
    else if (e.type == SDL_EVENT_KEY_DOWN)
    {
        // TODO: Gérer les touches spéciales
        // Backspace, Enter, Flèches...
        switch (e.key.key)
        {
            case SDLK_BACKSPACE:
                if (cursor_pos_ > 0)
                {
                    text_.erase(cursor_pos_ - 1, 1);
                    --cursor_pos_;
                }
                break;
            case SDLK_RETURN:
                if (!text_.empty())
                {
                    std::cout << "History size: " << history_->size() << "\n";
                    std::cout << "Adding to history: " << text_ << "\n";
                    history_->insertBack(text_);
                    std::cout << "History size: " << history_->size() << "\n";
                    // Parse & syntax-check
                    Command* cmd = queue_handler_->parse_and_validate(text_);
                    if (cmd)
                    {
                        cmd->pre_execute(queue_handler_);
                        if (cmd->is_edit_command()) delete cmd;  // Clean up if it's an edit command
                    }
                    else
                    {
                        cout << "[Invalid Command]\n";
                    }
                    text_.clear();
                    cursor_pos_ = 0;
                    history_index_ = -1;
                }
                break;
        case SDLK_LEFT:
            if (cursor_pos_ > 0)
            {
                --cursor_pos_;
            }
            break;
        case SDLK_RIGHT:
            if (cursor_pos_ < text_.size())
            {
                ++cursor_pos_;
            }
            break;
        case SDLK_UP:
            if (history_index_ < history_->size() - 1)
            {
                ++history_index_;
                auto it = history_->end();
                for (int i = 0; i < history_index_ + 1; ++i) --it;
                text_ = *it;
                cursor_pos_ = text_.size();
            }
            //cout << history_index_ << "\n";
            break;
		case SDLK_DOWN:
            if (history_index_ > 0)
            {
                --history_index_;
                auto it = history_->end();
                for (int i = 0; i < history_index_ + 1; ++i) --it;
                text_ = *it;
                cursor_pos_ = text_.size();
            }
            else if (history_index_ == 0)
            {
                history_index_ = -1;
                text_.clear();
                cursor_pos_ = 0;
            }
            //cout << history_index_ << "\n";
            break;
		default:
            break;
        }
    }
}

inline void TextBox::render(SDL_Renderer* renderer)
{
    Uint64 current_time = SDL_GetTicks();
    if (current_time - last_cursor_toggle_time_ >= 500) {  // 500ms
        cursor_visible_ = !cursor_visible_;
        last_cursor_toggle_time_ = current_time;
    }
    // Dessiner le fond de la TextBox (gris foncé)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_FRect box_rect = {
        static_cast<float>(box_pos_.x),
        static_cast<float>(box_pos_.y),
        static_cast<float>(width_),
        static_cast<float>(height_)
    };
    SDL_RenderFillRect(renderer, &box_rect);
    if (cursor_visible_) {
    // Calculer la position X du curseur
    int cursor_x = box_pos_.x + 5;  // Commencer au padding
    
    if (cursor_pos_ > 0 && !text_.empty()) {
    std::string text_before_cursor = text_.substr(0, cursor_pos_);
    
    // Créer une surface temporaire pour mesurer la largeur
    SDL_Color temp_color = {255, 255, 255, 255};
    SDL_Surface* temp_surface = TTF_RenderText_Solid(font_, 
                                                      text_before_cursor.c_str(), 
                                                      text_before_cursor.size(), 
                                                      temp_color);
    if (temp_surface) {
        cursor_x += temp_surface->w;
        SDL_DestroySurface(temp_surface);
    }
}
    
    // Dessiner une ligne verticale blanche
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanc
    SDL_RenderLine(renderer, 
                   cursor_x, box_pos_.y + 5,           // Point de départ (haut)
                   cursor_x, box_pos_.y + height_ - 5  // Point d'arrivée (bas)
    );
}
    if (!text_.empty()) {
        SDL_Color text_color = {255, 255, 255, 255};
        SDL_Surface* text_surface = TTF_RenderText_Solid(font_, text_.c_str(), text_.size(), text_color);
    
        if (!text_surface) {
            std::cout << "Failed to create text surface: " << SDL_GetError() << "\n";
            return;
        }
    
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    
        if (!text_texture) {
            std::cout << "Failed to create text texture: " << SDL_GetError() << "\n";
            SDL_DestroySurface(text_surface);
            return;
        }
    
        // Afficher les dimensions pour déboguer
        //std::cout << "Text surface: w=" << text_surface->w << " h=" << text_surface->h << "\n";
    
    
        SDL_FRect text_rect = {
            static_cast<float>(box_pos_.x + 5),  // Petit padding
            static_cast<float>(box_pos_.y + 5),
            static_cast<float>(text_surface->w),
            static_cast<float>(text_surface->h)
        };
    
        SDL_RenderTexture(renderer, text_texture, NULL, &text_rect);
    
        SDL_DestroySurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }
}