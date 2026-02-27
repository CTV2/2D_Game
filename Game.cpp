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




// Global variablesq
Map* map;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
auto& Player(manager.addEntity());

// Constructor and Destructor  < -------- Not sure if these are necessary, but they are here for now.
Game::Game() {
}
Game::~Game() {
}

// Creates instance of game
void Game::init(const char *title,int width, int height, bool fullscreen) {

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
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
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
                break;
            default:
                break;
        }
    }


}

// Checks for collision between player and map objects
bool Game::collision_player(PositionComponent Player) {
    for (GameObject& obj : map->objects) {
        // Simple AABB collision detection
        if (Player.x() < obj.GetXPos() + 64 &&
            Player.x() + 16 > obj.GetXPos() &&
            Player.y() < obj.GetYPos() + 64 &&
            Player.y() + 16 > obj.GetYPos()) {
                
            // Collision detected, handle accordingly (e.g., stop movement, change texture, etc.)
            std::cout << "Collision Detected!" << std::endl;
            
            return true;
        }
    }
    return false;

}


// Updates game state
void Game::update() {

    // moves the player down at a constant rate until hitting bottom of screen
    auto& position = Player.getComponent<PositionComponent>();
    if (position.y() < 480 - fallSpeed) {
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
    collision_player(position);
    manager.refresh();
    manager.update();
}

// Renders to screen
void Game::render() {
    // Clear previous render
    SDL_RenderClear(renderer);

    // Add items to render
    map->Draw_OBJ();
    manager.draw();
    SDL_RenderPresent(renderer);
}

// Cleans up memory and quits SDL
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game Closed" << std::endl;
}
