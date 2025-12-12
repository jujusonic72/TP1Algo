#include <SDL3/SDL.h>
#include <thread>
#include <atomic>
#include <iostream>

#include "../include/Player.h"
#include "../include/QueueHandler.h"
#include "../include/Command.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Camera.h"
#include "../include/TextBox.h"
#include <SDL3_image/SDL_image.h>

/*TODO:
- Faire une classe Enemy
- Faire une classe Item
- Faire un inventaire pour le joueur
- Faire apparaître les ennemis dans le monde aléatoirement
- Faire render les ennemis dans le monde un fois qu'il sont spawned
- Faire une classe UseCommand pour utiliser un item dans l'inventaire
- Faire une classe AtackCommand avec un cooldown qui fait des dégats aux ennemis dans la range et qui prompt les ennemis de faire des dégats au joueur
- Faire en sorte que les ennemis drop des item en mourant
- Faire une classe GatherCommand qui pick up les items dans la range et les ajoute à l'inventaire du joueur
- Faire un document de synthèse pour le tp
*/
std::atomic<bool> running(true);

void RefreshRender(Player* player)
{
    
}

int main(int argc, char* argv[])
{
    const int window_width = 800;
    const int window_height = 600;
    std::cout << "Starting application...\n";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    std::cout << "SDL initialized successfully\n";

    SDL_Window* window = SDL_CreateWindow(
        "Ma première fenêtre SDL3",
        window_width, window_height,
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

    // Charger le sprite du joueur
if (!player->loadSprite(renderer, "./assets/sprites/Player.png")) {
    std::cout << "Warning: Could not load player sprite\n";
}
    Camera camera(window_width, window_height, player);
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
        // Dessiner le joueur
        Position player_world = player->getPosition();
        std::cout << "Player world pos: (" << player_world.x << ", " << player_world.y << ")\n";
        Position player_screen = camera.pos_to_screen(player_world);
        std::cout << "Player screen pos: (" << player_screen.x << ", " << player_screen.y << ")\n";

        player->render(renderer, player_screen);
        text_box.render(renderer);
        
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Attend 1/60 de seconde
    }
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}