#include "Game.hpp"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Component.h"


Map* map;

SDL_Renderer* Game::renderer = nullptr;
Manager manager;
auto& Player(manager.addEntity());

Game::Game() {

}
Game::~Game() {

}

void Game::init(const char *title,int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
    }
    else {
        std::cout << "game running" << std::endl;
        window = SDL_CreateWindow(title,width,height,flags);
        if (window) {
            std::cout << "Window Created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window,NULL);
        if (renderer) {
            std::cout << "Render Created" << std::endl;
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            isRunning = true;
        }
        else {
            SDL_Log("SDL_render failed: %s", SDL_GetError());
            isRunning = false;
        }
        map = new Map();
        Player.addComponent<PositionComponent>(0,450);
        Player.addComponent<SpriteComponent>("textures/Reaper.png");
    }



}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }


}

void Game::update() {
    manager.refresh();
    manager.update();


}

void Game::render() {
    SDL_RenderClear(renderer);
    // Add items to render
    map -> DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game Closed" << std::endl;
}


