// Declares the Game class interface and shared renderer used across the project.
#ifndef Game_hpp
#define Game_hpp

#include "SDL3/SDL.h"
#include  "SDL3_image/SDL_image.h"
#include "ECS/Component.h"
#include <iostream>
#include <vector>

class Map;

// Define class for start menu
enum class GameState {
    MENU,
    PLAYING,
    QUIT
};

class Game {
public:
    // Temp stuff for testing, will be removed later
    float fallSpeed = 0.5; // change this to speed or slow
    GameState currentState = GameState::MENU;

    Game();
    ~Game();
    
    void init(const char* title,int width, int height, bool fullscreen);
    bool collision_player(PositionComponent Player);
    void handleEvents();
    void update();
    void collision_player();
    void render();
    void clean(); //mem cleaner
    bool running() {return isRunning;} // continue loop

    static SDL_Renderer* renderer;
private:
    int count;
    bool isRunning;
    SDL_Window * window;
    Map* map;
};


#endif
