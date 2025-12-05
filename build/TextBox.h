#pragma once

#include <stdexcept>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>

#include "Command.h"
#include "NodeList.h"
#include "Position.h"
using namespace std;
class TextBox
{
public:
    TextBox();
    ~TextBox();
    void handle_event(const SDL_Event& e);
    void render(SDL_Renderer* renderer);
private:
    string text_;
    NodeList<string> history_;
    TTF_Font* font_;
    Position box_pos_{0, 0};
    bool cursor_visible_ = true;
    Uint64 last_cursor_toggle_time_ = 0;
    int font_size_, height_, width_, cursor_pos_, history_index_ = -1;
};
inline TextBox::TextBox() : font_size_(12), height_(30), width_(400), cursor_pos_(0)
{
    if (!(font_ = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24))) throw std::runtime_error("Failed to load font");
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
                    // Parse & syntax-check
                    Command* cmd = queueHandler->parse_and_validate(line);
                    if (cmd)
                    {
                        cmd->pre_execute(queueHandler);
                        if (cmd->is_edit_command()) delete cmd;  // Clean up if it's an edit command
                    }
                    else
                    {
                        std::cout << "[Invalid Command]\n";
                    }
                }
        }
    }
}