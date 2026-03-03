#include "Game.hpp"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Component.h"
#include <vector>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>



bool reset  = false;
int score = 0;
int final_score = 0;
int speed_change = 0;
static TTF_Font *font = NULL;


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
Entity* Player = nullptr;

std::vector<Entity*> Tree_list;
std::vector<Entity*> Asteroid_list;
int asteroid_spawn_goal = 0;


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

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        SDL_Log("TTF failed: %s", SDL_GetError());
    }

    /* Open the font */
    font = TTF_OpenFont("textures/buzz.ttf", 150);
    if (!font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());

    }
    // Sets game state to menu
    currentState = GameState::MENU;

    // Sets up flags for SDL (Just fullscreen for now, but can be expanded later if needed)
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initializes SDL, error checks, and creates window and renderer. Also initializes map and player entity with position and sprite components.
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
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


    }

    if (isRunning) {
        background = TextureManager::LoadTexture("textures/sky.jpg");
        if (!background) {
            SDL_Log("Failed to load background: %s", SDL_GetError());
        }
    }

    if (isRunning) {
        if (!MIX_Init()) {
            SDL_Log("MIX_Init failed: %s", SDL_GetError());
        } else {
            mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (!mixer) {
                SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
            } else {
                musicAudio = MIX_LoadAudio(mixer, "textures/Nightmare.ogg", true);
                if (!musicAudio) {
                    SDL_Log("MIX_LoadAudio failed: %s", SDL_GetError());
                } else {
                    musicTrack = MIX_CreateTrack(mixer);
                    if (!musicTrack) {
                        SDL_Log("MIX_CreateTrack failed: %s", SDL_GetError());
                    } else if (!MIX_SetTrackAudio(musicTrack, musicAudio)) {
                        SDL_Log("MIX_SetTrackAudio failed: %s", SDL_GetError());
                    } else {
                        MIX_SetTrackLoops(musicTrack, -1);
                        if (!MIX_PlayTrack(musicTrack, 0)) {
                            SDL_Log("MIX_PlayTrack failed: %s", SDL_GetError());
                        }
                    }
                }
            }
        }
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
                        Player = &manager.addEntity();
                        // Initialize map and player entity with position and sprite components

                        Player->addComponent<PositionComponent>();
                        auto& playerSprite = Player->addComponent<SpriteComponent>("textures/astro.png", 0.14f);
                        const SDL_FRect& playerDest = playerSprite.getDestRect();

                        // Keep a tighter hitbox inside the sprite bounds for fair collision.
                        const float hitboxInsetX = playerDest.w * 0.22f;
                        const float hitboxInsetY = playerDest.h * 0.18f;
                        float hitboxWidth = playerDest.w - (2.0f * hitboxInsetX);
                        float hitboxHeight = playerDest.h - (2.0f * hitboxInsetY);
                        if (hitboxWidth < 1.0f) hitboxWidth = 1.0f;
                        if (hitboxHeight < 1.0f) hitboxHeight = 1.0f;
                        Player->addComponent<ColliderComponent>(hitboxWidth, hitboxHeight, hitboxInsetX, hitboxInsetY);
                        Game::randomSpawn();
            

                    }
                    if (event.key.key == SDLK_ESCAPE) {
                        isRunning = false;
                    }
                }
                else if(currentState == GameState::PLAYING) {
                        // PLAYING controls
                    if (event.key.key == SDLK_W) {
                        //jump, change logic later to say if not falling
                        auto& position = Player->getComponent<PositionComponent>();
                        if(fallSpeed >= 1.0 && position.y() > fallSpeed && reset == true) {
                            fallSpeed = -30.0;
                            reset = false;
                        }
                    }
                    if (event.key.key == SDLK_A) {
                        //move left
                        auto& position = Player->getComponent<PositionComponent>();
                        const float moveDist = 20.0; // change this to speed or slow
                        if (position.x() > -30 - moveDist) {
                            position.setPos(position.x() - moveDist, position.y());
                            //std::cout << position.x() << std::endl;
                        }
                    }
                    if (event.key.key == SDLK_D) {
                        //move right
                        auto& position = Player->getComponent<PositionComponent>();
                        const float moveDist =20.0; // change this to speed or slow
                        if (position.x() < 1280 - moveDist) {
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
    if (!Player->hasComponent<ColliderComponent>()) {
        return;
    }

    const SDL_FRect Player_col = Player->getComponent<ColliderComponent>().getCollider();

    // Here is the list of objects we want to check for collision with. We can add more as needed, but for now we will just check the trees and spikes.
    // Entity* check[] = {Tree_0}; //  {&Tree_0, &Tree_1, &Tree_2}
    std::vector<Entity*> combined;

    combined.insert(combined.end(), Tree_list.begin(), Tree_list.end());
    combined.insert(combined.end(), Asteroid_list.begin(), Asteroid_list.end());

    for (Entity* e : combined) {
        if (!e->hasComponent<ColliderComponent>()) {
            continue;
        }
        const SDL_FRect checkRect = e->getComponent<ColliderComponent>().getCollider();
        if (collides(Player_col,checkRect)) {
            currentState = GameState::MENU;
            fallSpeed = 0;
            manager.clear();
            Tree_list.clear();
            Asteroid_list.clear();  
            std::cout << score << std::endl;
            final_score = score;
            score = 0;
            asteroid_spawn_goal = 0;

            break;


        }
    }


}


// Updates game state
void Game::update() {
    if (currentState != GameState::PLAYING)
        return;


    // moves the player down at a constant rate until hitting bottom of screen
    auto& position = Player->getComponent<PositionComponent>();
    if (position.y() < 480 - fallSpeed) {
        //Dont update if jumping above screen
        if(!(fallSpeed < 0 && position.y() < fallSpeed)){
            position.setPos(position.x(), position.y() + fallSpeed);
        }

        //std::cout << position.y() << std::endl;
    }
    else {
        reset = true;
    }
    //This controls the fastest falling. Adjust the if statement to make faster/slower max fall
    if (fallSpeed < 10.0) {
        fallSpeed += 1.0;
    }
    
    // Moved these updates to seperate functions to avoid cluttering the main update function and to allow for different update logic for different object types if needed in the future.
    if (score < 100) { float speed_change = float(int(score / 5));}
    

    Game::updateTrees();
    asteroid_spawn_goal = Game::updateAsteroids(asteroid_spawn_goal);
    
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
    if (background) {
        SDL_FRect bgDest = {0.0f, 0.0f, 1280.0f, 720.0f}; // match your window size
        SDL_RenderTexture(renderer, background, nullptr, &bgDest);
    }
    if (currentState == GameState::MENU) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Color bgColor   = {0, 0, 0, 225};
        char score_buf[32];
        SDL_snprintf(score_buf, sizeof(score_buf), "Score: %d", final_score);

        if (font) {
            SDL_Surface* text = TTF_RenderText_LCD(font, "Press Enter To Play", 0, textColor,bgColor);
            if (text) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, text);
                SDL_DestroySurface(text);
                if (tex) {
                    SDL_FRect textDest = {350.0f, 250.0f, 500.0f, 200.f};
                    SDL_RenderTexture(renderer, tex, nullptr, &textDest);
                    SDL_DestroyTexture(tex);
                }
            }

            SDL_Surface* menu_score = TTF_RenderText_LCD(font, score_buf, 0, textColor, bgColor);
            if (menu_score) {
                SDL_Texture* score_tex = SDL_CreateTextureFromSurface(renderer, menu_score);
                SDL_DestroySurface(menu_score);
                if (score_tex) {
                    SDL_FRect scoreDest = {500.0f, 440.0f, 200.0f, 60.0f};
                    SDL_RenderTexture(renderer, score_tex, nullptr, &scoreDest);
                    SDL_DestroyTexture(score_tex);
                }
            }
        }


    }

    if (currentState == GameState::PLAYING) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Color bgColor   = {0, 0, 0, 225};
        char score_buf[32];
        SDL_snprintf(score_buf, sizeof(score_buf), "Score: %d", score);
        if (font) {
            SDL_Surface* text = TTF_RenderText_LCD(font, score_buf, 0, textColor,bgColor);
            if (text) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, text);
                SDL_DestroySurface(text);
                if (tex) {
                    SDL_FRect textDest = {1150, 15.0f, 100.0f, 40.f};
                    SDL_RenderTexture(renderer, tex, nullptr, &textDest);
                    SDL_DestroyTexture(tex);
                }
            }
        }


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
    if (background) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }
    if (musicTrack) {
        MIX_DestroyTrack(musicTrack);
        musicTrack = nullptr;
    }
    if (musicAudio) {
        MIX_DestroyAudio(musicAudio);
        musicAudio = nullptr;
    }
    if (mixer) {
        MIX_DestroyMixer(mixer);
        mixer = nullptr;
    }
    MIX_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game Closed" << std::endl;
}

void Game::spawnTree() {

    Entity* newTree = &manager.addEntity();

    // Add components
    newTree->addComponent<PositionComponent>(1200, 480);
    newTree->addComponent<ColliderComponent>(120, 120, 15, 10);
    newTree->addComponent<SpriteComponent>("textures/up_tree.png", 0.50);

    // Add to the tree list
    Tree_list.push_back(newTree);
}

void Game::spawnSpike() {

    Entity* newSpike = &manager.addEntity();

    // Add components Done By AI Because Changing sprite last minute changed hitboxes
    newSpike->addComponent<PositionComponent>(1200, 645);
    auto& spikeSprite = newSpike->addComponent<SpriteComponent>("textures/Spikes.png", 0.50f);
    const SDL_FRect& spikeDest = spikeSprite.getDestRect();

    // Tighten collision to the visible spike tips to avoid early hits.
    const float hitboxInsetX = spikeDest.w * 0.22f;
    const float hitboxTopInset = spikeDest.h * 0.48f;
    const float hitboxBottomInset = spikeDest.h * 0.08f;
    float hitboxWidth = spikeDest.w - (2.0f * hitboxInsetX);
    float hitboxHeight = spikeDest.h - hitboxTopInset - hitboxBottomInset;
    if (hitboxWidth < 1.0f) hitboxWidth = 1.0f;
    if (hitboxHeight < 1.0f) hitboxHeight = 1.0f;
    newSpike->addComponent<ColliderComponent>(hitboxWidth, hitboxHeight, hitboxInsetX, hitboxTopInset);

    // Add to the tree list
    Tree_list.push_back(newSpike);

}

void Game::spawnAsteroid() {
    Entity* newAsteroid = &manager.addEntity();

    // Add components
    float asteroidY = rand() % 300 + 25; // Random Y position between 50 and 450
    newAsteroid->addComponent<PositionComponent>(1400, asteroidY);
    newAsteroid->addComponent<ColliderComponent>(50, 50, 0, 0);
    newAsteroid->addComponent<SpriteComponent>("textures/asteroid.png", 0.20f);

    // Add to the asteroid list
    Asteroid_list.push_back(newAsteroid);
}

void Game::randomSpawn() {
    int randNum = rand() % 2; // Random number between 0 and 1
    if (randNum == 0) {
        spawnTree();
    } else {
        spawnSpike();
    }
}


int Game::updateAsteroids(int goal) {
   
    
    if (score > 4 and goal <= 0) { // Spawn asteroids every 10 seconds
        spawnAsteroid();
        if (score < 29) { goal = rand() % (1000 - (speed_change * 100) - 400 + 1) + 400;}
        else { goal = rand() % 300 + 100; } // Cap spawn rate at higher scores to avoid overwhelming the player.

    }
    else {
        goal -= 1;
    }

    for (int i = Asteroid_list.size() - 1; i >= 0; i--) {
        Entity* asteroid = Asteroid_list[i];
        auto& pos = asteroid->getComponent<PositionComponent>();
        
        pos.setPos(pos.x() - 9 - speed_change, pos.y());
        if (pos.x() < -400) {
            asteroid->destroy();
            score += 1;
            Asteroid_list.erase(Asteroid_list.begin() + i);
        }
    }
    return goal ;
}

void Game::updateTrees() {
    
    for (int i = Tree_list.size() - 1; i >= 0; i--) {
        Entity* tree = Tree_list[i];
        auto& pos = tree->getComponent<PositionComponent>();
        

        pos.setPos(pos.x() - 5 - speed_change, pos.y());
        if (pos.x() < -400) {
            tree->destroy();
            score += 1;
            Tree_list.erase(Tree_list.begin() + i);
            Game::randomSpawn();
        }
    }
}