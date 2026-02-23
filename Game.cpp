#include "Game.hpp"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Component.h"

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
        map = new Map();
        Player.addComponent<PositionComponent>(0,450);
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
            default:
                break;
        }
    }


}

// Updates game state
void Game::update() {
    manager.refresh();
    manager.update();
}

// Renders to screen
void Game::render() {
    // Clear previous render
    SDL_RenderClear(renderer);
    
    // Add items to render
    map -> DrawMap();
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


