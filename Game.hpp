// Declares the Game class interface and shared renderer used across the project.
#ifndef Game_hpp
#define Game_hpp

#include "SDL3/SDL.h"
#include  "SDL3_image/SDL_image.h"
#include <iostream>
#include <vector>

class Game {
public:
    // Temp stuff for testing, will be removed later


    Game();
    ~Game();
    void init(const char* title,int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean(); //mem cleaner
    bool running() {return isRunning;} // continue loop

    static SDL_Renderer* renderer;
private:
    int count;
    bool isRunning;
    SDL_Window * window;
};
#endif
