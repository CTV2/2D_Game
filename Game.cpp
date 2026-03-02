#include "Game.hpp"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Component.h"
#include <vector>




// Temp stuff for testing, will be removed later
std::vector<std::vector<int>> generateMap() {
    const int rows = 20;
    const int cols = 25;

    std::vector<std::vector<int>> array(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        int value;
        if (i > 5)
            value = 0;
        else if (i > 2)
            value = 1;
        else
            value = 2;

        for (int j = 0; j < cols; ++j) {
            array[i][j] = value;
        }
    }

    return array;
}




SDL_Renderer* Game::renderer = nullptr;
Manager manager;
auto& Player(manager.addEntity());
auto& Tree_0(manager.addEntity());
auto& Tree_1(manager.addEntity());
auto& Tree_2(manager.addEntity());
auto& Spikes(manager.addEntity());

// Constructor and Destructor  < -------- Not sure if these are necessary, but they are here for now.
Game::Game() {
    map = nullptr;
}
Game::~Game() {
}

bool collides(const SDL_FRect& a, const SDL_FRect& b) {
    if (a.x + a.w <= b.x || b.x + b.w <= a.x) return false;
    if (a.y + a.h <= b.y || b.y + b.h <= a.y) return false;
    return true;

}


// Creates instance of game
void Game::init(const char *title,int width, int height, bool fullscreen) {


    // Sets game state to menu
    GameState currentState = GameState::MENU;

    // Sets up flags for SDL (Just fullscreen for now, but can be expanded later if needed)
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initializes SDL, error checks, and creates window and renderer. Also initializes map and player entity with position and sprite components.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // Error Checking
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
    }
    else {
        // Initialization successful, create window and renderer

        std::cout << "game running" << std::endl;

        // Creates window
        window = SDL_CreateWindow(title,width,height,flags);
        if (window) {
            std::cout << "Window Created" << std::endl;
        } else {
            // Error
            SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
            isRunning = false;
        }

        // Creates renderer
        renderer = SDL_CreateRenderer(window,NULL);
        if (renderer) {
            std::cout << "Render Created" << std::endl;
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            isRunning = true;
        }
        else {
            // Error
            SDL_Log("SDL_render failed: %s", SDL_GetError());
            isRunning = false;
        }

        // Initialize map and player entity with position and sprite components
        map = new Map(generateMap());
        Player.addComponent<PositionComponent>();
        Player.addComponent<ColliderComponent>(16, 16);
        Tree_0.addComponent<PositionComponent>(0,430);
        Tree_1.addComponent<PositionComponent>(500,430);
        Tree_2.addComponent<PositionComponent>(1000,430);
        Tree_0.addComponent<ColliderComponent>(120, 120, 15, 10);
        Tree_1.addComponent<ColliderComponent>(70, 120, 15, 10);
        Tree_2.addComponent<ColliderComponent>(70, 120, 15, 10);
        Spikes.addComponent<PositionComponent>();
        //Spikes.addComponent<SpriteComponent>("textures/spikes.png");
        Tree_0.addComponent<SpriteComponent>("textures/up_tree.png",0.50);
        Tree_1.addComponent<SpriteComponent>("textures/up_tree.png",0.50);
        Tree_2.addComponent<SpriteComponent>("textures/up_tree.png",0.50);
        Player.addComponent<SpriteComponent>("textures/Reaper.png");
    }
}


// Handles events, currently just checks for quit event
void Game::handleEvents() {
    SDL_Event event;

    // See if there is an event to process
    while (SDL_PollEvent(&event)) {

        // Check event type and handle accordingly
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            //key is pressed
            case SDL_EVENT_KEY_DOWN:
                // MENU controls
                if (currentState == GameState::MENU) {
                    if (event.key.key == SDLK_RETURN) {
                        currentState = GameState::PLAYING;
                    }
                    if (event.key.key == SDLK_ESCAPE) {
                        isRunning = false;
                    }
                }
                else if(currentState == GameState::PLAYING) {
                        // PLAYING controls
                    if (event.key.key == SDLK_W) {
                        //jump, change logic later to say if not falling
                        auto& position = Player.getComponent<PositionComponent>();
                        if(fallSpeed >= 1.0 && position.y() > fallSpeed){
                            fallSpeed = -10.0;
                        }
                    }
                    if (event.key.key == SDLK_A) {
                        //move left
                        auto& position = Player.getComponent<PositionComponent>();
                        const float moveDist = 5.0; // change this to speed or slow
                        if (position.x() > -30 - moveDist) {
                            position.setPos(position.x() - moveDist, position.y());
                            //std::cout << position.x() << std::endl;
                        }
                    }
                    if (event.key.key == SDLK_D) {
                        //move right
                        auto& position = Player.getComponent<PositionComponent>();
                        const float moveDist = 5.0; // change this to speed or slow
                        if (position.x() < 665 - moveDist) {
                        position.setPos(position.x() + moveDist, position.y());
                        //std::cout << position.x() << std::endl;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }


}

// Checks for collision between player and map objects
void Game::collision_player() {
    if (!Player.hasComponent<ColliderComponent>()) {
        return;
    }
    const SDL_FRect Player_col = Player.getComponent<ColliderComponent>().getCollider();
    Entity* check[] = {&Tree_0, &Tree_1, &Tree_2};
    for (Entity* e : check) {
        if (!e->hasComponent<ColliderComponent>()) {
            continue;
        }
        const SDL_FRect checkRect = e->getComponent<ColliderComponent>().getCollider();
        if (collides(Player_col,checkRect)) {
            std::cout<<"Player collided"<<std::endl;
        }
    }


}


// Updates game state
void Game::update() {

    if (currentState != GameState::PLAYING)
        return;

    // moves the player down at a constant rate until hitting bottom of screen
    auto& position = Player.getComponent<PositionComponent>();
    if (position.y() < 550 - fallSpeed) {
        //Dont update if jumping above screen
        if(!(fallSpeed < 0 && position.y() < fallSpeed)){
            position.setPos(position.x(), position.y() + fallSpeed);
        }

        //std::cout << position.y() << std::endl;
    }
    //This controls the fastest falling. Adjust the if statement to make faster/slower max fall
    if (fallSpeed < 10.0) {
        fallSpeed += 1.0;
    }
    // Temporarily ignoring map collisions while testing player behavior.
    // collision_player();
    manager.refresh();
    manager.update();
    collision_player();
}

// Renders to screen
void Game::render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    // Clear previous render
    SDL_RenderClear(renderer);

    if (currentState == GameState::MENU) {

        // Simple menu background
        SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);
        SDL_FRect rect = { 200, 200, 400, 200 };
        SDL_RenderFillRect(renderer, &rect);
    }
    else if (currentState == GameState::PLAYING) {
        // Add items to render
        // Temporarily ignoring map rendering.
        // map->Draw_OBJ();
        manager.draw();
    }

    SDL_RenderPresent(renderer);
}

// Cleans up memory and quits SDL
void Game::clean() {
    delete map;
    map = nullptr;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game Closed" << std::endl;
}
//t