#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <thread>
#include <atomic>
#include <iostream>


#include <SDL3_ttf/SDL_ttf.h>
#include "Camera.h"
#include "Pnj.h"
#include "TextBox.h"
#include <SDL3_image/SDL_image.h>
#include "Player.h"
#include "QueueHandler.h"
#include "Command.h"
#include "NodeList.h"
#include "Enemy.h"
#include "Position.h"
#include "Item.h"
#include "Items/ItemHealthPotion.h"
#include "Items/ItemDamageBoost.h"
#include "Items/ItemSpeedBoost.h"
#include "Inventory.h"

/*TODO:
- Faire une classe Item ✓
- Faire un inventaire pour le joueur ✓
- Faire apparaître les ennemis dans le monde aléatoirement
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
void renderDebugText(SDL_Renderer* renderer, TTF_Font* font, Position screen_pos, const std::string& text)
{
    SDL_Color text_color = {255, 255, 0, 255};  // Jaune
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), text_color);
    
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        SDL_FRect rect = {
            static_cast<float>(screen_pos.x - surface->w / 2),
            static_cast<float>(screen_pos.y + 30),  // 30 pixels sous l'entité
            static_cast<float>(surface->w),
            static_cast<float>(surface->h)
        };
        
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
        
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
    }
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
    

    // Spawn quelques ennemis
    queueHandler.add_enemy(new Enemy(50, 10, 2000, 100, {200, 200}));
    queueHandler.add_enemy(new Enemy(50, 10, 2000, 100, {-200, -200}));
    queueHandler.add_enemy(new Enemy(50, 10, 2000, 100, {300, 150}));
    
    // NOUVEAU: Liste des items dans le monde
    NodeList<Item*> world_items;
    
    // Spawner des items de test
    Item* potion1 = new ItemHealthPotion("Health Potion", "Restores 50 HP", 50);
    potion1->setPosition({100, 100});
    potion1->setOnGround(true);
    world_items.insertBack(potion1);
    
    Item* potion2 = new ItemHealthPotion("Health Potion", "Restores 50 HP", 50);
    potion2->setPosition({-150, 200});
    potion2->setOnGround(true);
    world_items.insertBack(potion2);
    
    Item* damage_boost = new ItemDamageBoost("Damage Boost", "Increases damage by 10 for 30 seconds", 10, 30000);
    damage_boost->setPosition({200, -100});
    damage_boost->setOnGround(true);
    world_items.insertBack(damage_boost);
    
    Item* speed_boost = new ItemSpeedBoost("Speed Boost", "Increases speed by 20 for 30 seconds", 20, 30000);
    speed_boost->setPosition({-200, -150});
    speed_boost->setOnGround(true);
    world_items.insertBack(speed_boost);
    
    
    // Donner quelques items au joueur pour tester l'inventaire
    player->pickupItem(new ItemHealthPotion("Health Potion", "Restores 50 HP", 50));
    player->pickupItem(new ItemSpeedBoost("Speed Boost", "Increases speed by 20 for 30 seconds", 20, 30000));

    // Charger le sprite du joueur
    if (!player->loadSprite(renderer, "./assets/sprites/Hero.png", 8)) {
        std::cout << "Warning: Could not load player sprite\n";
    }
    
    Camera camera(window_width, window_height, player);
    queueHandler.set_player(player);
    TextBox text_box(&queueHandler, 10, window_height - 50, 400, 40);
    // Charger le sprite des ennemis
    if(!queueHandler.get_enemies().empty())
    {
        auto& enemies_list = queueHandler.get_enemies();
        auto it = enemies_list.begin();
        while (it != enemies_list.end()) {
            Enemy* enemy = *it;
            enemy->loadSprite(renderer, 5);
            ++it;
        }
    }
    
    queueHandler.set_pnj(new Pnj({0, 200}));
    queueHandler.get_pnj()->loadSprite(renderer, "./assets/sprites/Pnj.png", 7);
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
        player->update(delta_time);
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
            // if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_E) {
            // if (pnj->isPlayerInRange(player->getPosition())) {
            // pnj->interact();  // Passe au dialogue suivant
            // }
            //}
        }

        if (!queueHandler.isEmpty())
        {
            // Execute the current command
            queueHandler.front()->execute(delta_time);
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
        SDL_SetRenderDrawColor(renderer, 0, 123, 0, 255); // Fond vert
        SDL_RenderClear(renderer);
        SDL_GetWindowSize(window, &window_width, &window_height);

        Position pnj_screen = camera.pos_to_screen(queueHandler.get_pnj()->getPosition(), window_height, window_width);
        queueHandler.get_pnj()->render(renderer, pnj_screen);
        if (queueHandler.get_pnj()->isPlayerInRange(player->getPosition())) {
            queueHandler.get_pnj()->renderDialogue(renderer, text_box.getFont(), window_width, window_height);
        }   
        std::string pnj_pos_text = "(" + std::to_string(int(queueHandler.get_pnj()->getPosition().x)) + ", " + std::to_string(int(queueHandler.get_pnj()->getPosition().y)) + ")";
        renderDebugText(renderer, text_box.getFont(), {pnj_screen.x, pnj_screen.y + 20}, pnj_pos_text);

        // Dessiner les ennemis
        auto& enemies_list = queueHandler.get_enemies(); // reuse non-const ref
        for (auto it = enemies_list.begin(); it != enemies_list.end(); ++it) {
            Enemy* enemie = *it;
            Position enemy_screen = camera.pos_to_screen(enemie->getPosition(), window_height, window_width);
            enemie->render(renderer, enemy_screen);
            // Debug text pour l'ennemi
            std::string enemy_pos_text = "(" + std::to_string(int(enemie->getPosition().x)) + ", " + std::to_string(int(enemie->getPosition().y)) + ")";
            renderDebugText(renderer, text_box.getFont(), enemy_screen, enemy_pos_text); 
        }
        
        queueHandler.renderQueue(renderer, text_box.getFont(), 10, window_height - 100);
        // NOUVEAU: Dessiner les items au sol
        for (auto & item : world_items) {
            if (item->isOnGround()) {
                Position item_screen = camera.pos_to_screen(item->getPosition(), window_height, window_width);
                item->render(renderer, item_screen);
                std::string enemy_pos_text = "(" + std::to_string(int(item->getPosition().x)) + ", " + std::to_string(int(item->getPosition().y)) + ")";
                renderDebugText(renderer, text_box.getFont(), item_screen, enemy_pos_text); 
            }
        }
        // Dessiner le joueur
        Position player_world = player->getPosition();
        Position player_screen = camera.pos_to_screen(player_world, window_height, window_width);
        player->render(renderer, player_screen);
        std::string player_pos_text = "(" + std::to_string(int(player_world.x)) + ", " + std::to_string(int(player_world.y)) + ")";
        renderDebugText(renderer, text_box.getFont(), {player_screen.x, player_screen.y + 30}, player_pos_text);

        text_box.render(renderer);
        
        // NOUVEAU: Afficher l'UI du joueur (barre de vie + inventaire)
        player->renderUI(renderer, window_width, window_height);
        
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Attend 1/60 de seconde
    }

    // CLEANUP
    while (!queueHandler.get_enemies().empty()) 
    {
        auto& enemies_list_cleanup = queueHandler.get_enemies();
        Enemy* enemy = *enemies_list_cleanup.begin();
        delete enemy;
        enemies_list_cleanup.eraseFront();
    }
    
    // Cleanup des items
    while (!world_items.empty()) 
    {
        Item* item = *world_items.begin();
        delete item;
        world_items.eraseFront();
    }
    delete queueHandler.get_pnj();
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
