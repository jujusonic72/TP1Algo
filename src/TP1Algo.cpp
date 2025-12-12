#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <thread>
#include <atomic>
#include <iostream>


#include <SDL3_ttf/SDL_ttf.h>
#include "Camera.h"
#include "TextBox.h"
#include <SDL3_image/SDL_image.h>
#include "Player.h"
#include "QueueHandler.h"
#include "Command.h"
#include "NodeList.h"
#include "Enemy.h"
#include "Position.h"
#include "Item.h"
#include "Inventory.h"

/*TODO:
- Faire une classe Enemy ✓
- Faire une classe Item ✓
- Faire un inventaire pour le joueur ✓
- Faire apparaître les ennemis dans le monde aléatoirement
- Faire render les ennemis dans le monde un fois qu'il sont spawned ✓
- Faire une classe UseCommand pour utiliser un item dans l'inventaire
- Faire une classe AttackCommand avec un cooldown qui fait des dégâts aux ennemis dans la range et qui prompt les ennemis de faire des dégâts au joueur
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
    int window_width = 800;
    int window_height = 600;
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
    
    //Liste des ennemies
    NodeList<Enemy*> enemies;

    // Spawn quelques ennemis
    enemies.insertBack(new Enemy(50, 10, 2000, 100, {200, 200}));
    enemies.insertBack(new Enemy(50, 10, 2000, 100, {-200, -200}));
    enemies.insertBack(new Enemy(50, 10, 2000, 100, {300, 150}));
    
    // NOUVEAU: Liste des items dans le monde
    NodeList<Item*> world_items;
    
    // Spawner des items de test
    Item* potion1 = Item::createHealthPotion();
    potion1->setPosition({100, 100});
    potion1->setOnGround(true);
    world_items.insertBack(potion1);
    
    Item* potion2 = Item::createHealthPotion();
    potion2->setPosition({-150, 200});
    potion2->setOnGround(true);
    world_items.insertBack(potion2);
    
    Item* damage_boost = Item::createDamageBoost();
    damage_boost->setPosition({200, -100});
    damage_boost->setOnGround(true);
    world_items.insertBack(damage_boost);
    
    Item* speed_boost = Item::createSpeedBoost();
    speed_boost->setPosition({-200, -150});
    speed_boost->setOnGround(true);
    world_items.insertBack(speed_boost);
    
    
    // Donner quelques items au joueur pour tester l'inventaire
    player->pickupItem(Item::createHealthPotion());
    player->pickupItem(Item::createDamageBoost());

    // Charger le sprite du joueur
    if (!player->loadSprite(renderer, "./assets/sprites/Hero.png", 16)) {
        std::cout << "Warning: Could not load player sprite\n";
    }
    
    Camera camera(window_width, window_height, player);
    queueHandler.set_player(player);
    TextBox text_box = TextBox(&queueHandler);
    
    // Charger le sprite des ennemis
    if(!enemies.empty())
    {
        auto it = enemies.begin();
        while (it != enemies.end()) {
            Enemy* enemy = *it;
            enemy->loadSprite(renderer, 8);
            ++it;
        }      
    }
    
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
            }
        }

        if (!queueHandler.isEmpty())
        {
            // Execute the current command
            queueHandler.front()->execute(delta_time);
        }

        if(!enemies.empty())
        {
            auto it = enemies.begin();
            while (it != enemies.end()) {
                Enemy* enemy = *it;
                enemy->update(delta_time, player);
                ++it;
            }
        }
        
        //Logique de ramassage automatique des items proches
        Position player_pos = player->getPosition();
        auto item_it = world_items.begin();
        while (item_it != world_items.end()) {
            Item* item = *item_it;
            if (item->isOnGround()) {
                Position item_pos = item->getPosition();
                float dx = item_pos.x - player_pos.x;
                float dy = item_pos.y - player_pos.y;
                float distance = sqrt(dx*dx + dy*dy);
                
                // Ramassage automatique si à moins de 50 unités
                if (distance < 50.0f) {
                    if (player->pickupItem(item)) {
                        auto to_erase = item_it;
                        ++item_it;
                        world_items.erase(to_erase);
                        continue;
                    }
                }
            }
            ++item_it;
        }

        // RENDERING
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);
        SDL_GetWindowSize(window, &window_width, &window_height);
        
        // Dessiner le joueur
        Position player_world = player->getPosition();
        Position player_screen = camera.pos_to_screen(player_world, window_height, window_width);

        player->render(renderer, player_screen);
        
        // Dessiner les ennemis
        for (auto & enemie : enemies)
        {
            Position enemy_screen = camera.pos_to_screen(enemie->getPosition(), window_height, window_width);
            enemie->render(renderer, enemy_screen); 
        }
        
        // NOUVEAU: Dessiner les items au sol
        for (auto & item : world_items) {
            if (item->isOnGround()) {
                Position item_screen = camera.pos_to_screen(item->getPosition(), window_height, window_width);
                item->render(renderer, item_screen);
            }
        }

        text_box.render(renderer);
        
        // NOUVEAU: Afficher l'UI du joueur (barre de vie + inventaire)
        player->renderUI(renderer, window_width, window_height);
        
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Attend 1/60 de seconde
    }

    // CLEANUP
    while (!enemies.empty()) 
    {
        Enemy* enemy = *enemies.begin();
        delete enemy;
        enemies.eraseFront();
    }
    
    // NOUVEAU: Cleanup des items
    while (!world_items.empty()) 
    {
        Item* item = *world_items.begin();
        delete item;
        world_items.eraseFront();
    }
    
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
