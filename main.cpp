// Entry point that initializes the game, runs the fixed-FPS loop, and shuts down cleanly.
#include "Game.hpp"

Game *game = nullptr;
int main(int argc,const char * argv[]) {


    // Set up frame rate
    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    // Creates Instance of Game and initializes it
    game = new Game;
    game -> init("test",800,640,false);

    // Begins game loop
    while (game -> running()) {

        // Calculates frame time and delays if necessary to maintain frame rate
        frameStart = SDL_GetTicks();

        // Processes event list
        game -> handleEvents();

        // Updates game state and renders to screen
        game -> update();
        game -> render();

        // Calculates time taken for frame
        frameTime = SDL_GetTicks() - frameStart;

        // Delays if frame time is less than desired frame delay to maintain consistent frame rate
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay -frameTime);
        }
    }
    game-> clean();
    return 0;
}
