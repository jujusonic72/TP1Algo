#include <SDL3/SDL.h>
#include <thread>
#include <atomic>
#include <iostream>

#include "build/Player.h"
#include "build/QueueHandler.h"
#include "build/Command.h"
#include <SDL3_ttf/SDL_ttf.h>

#include "build/TextBox.h"

std::atomic<bool> running = true;

void RefreshRender(Player* player)
{
    
}

int main(int argc, char* argv[])
{
    std::cout << "Starting application...\n";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    std::cout << "SDL initialized successfully\n";

    SDL_Window* window = SDL_CreateWindow(
        "Ma première fenêtre SDL3",
        800, 600,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    std::cout << "Window created successfully\n";

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "Renderer created successfully\n";
    TTF_Init();
    QueueHandler queueHandler;
    Player* player = new Player();
    queueHandler.set_player(player);
    TextBox text_box = TextBox(&queueHandler);
    SDL_StartTextInput(window);
    
    
    std::cout << "Entering main loop. Type 'quit' or 'exit' to close, or close the window.\n";
    Uint64 last_time = SDL_GetTicks();
    // GAME LOOP
    SDL_Event event;
    while (running)
    {
        Uint64 current_time = SDL_GetTicks();
        Uint64 delta_time = current_time - last_time;
        last_time = current_time;
        
        // Process SDL events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_TEXT_INPUT || event.type == SDL_EVENT_KEY_DOWN)
            {
                text_box.handle_event(event);
                //std::cout << "listening to stuff" << " text: " << SDL_EVENT_TEXT_INPUT << ", Key: " << SDL_EVENT_KEY_DOWN << " " << event.type << "\n";
            }
        }

        if (!queueHandler.isEmpty())
        {
            // Execute the current command
            queueHandler.front()->execute(delta_time);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        text_box.render(renderer);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Attend 1/60 de seconde
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}