#include <SDL3/SDL.h>
#include <thread>
#include <atomic>
#include <iostream>

#include "build/Player.h"
#include "build/QueueHandler.h"
#include "build/Command.h"
#include <SDL3_ttf/SDL_ttf.h>

std::atomic<bool> running = true;

void InputThreadFunc(QueueHandler* queueHandler)
{
    while (running)
    {
        std::string line;
        std::getline(std::cin, line);

        if (line == "quit" || line == "exit") {
            running = false;
            break;
        }

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

    QueueHandler queueHandler;
    Player* player = new Player();

    // Starting the Input thread
    std::thread inputThread(InputThreadFunc, &queueHandler);
    inputThread.detach();  // Détache le thread pour qu'il ne bloque pas la fermeture
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
        }

        if (!queueHandler.isEmpty())
        {
            // Execute the current command
            queueHandler.front()->execute(delta_time);
        }

        SDL_Delay(16);  // Attend 1/60 de seconde
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}