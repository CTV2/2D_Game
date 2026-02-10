#ifndef INC_2DP_MAP_H
#define INC_2DP_MAP_H
#include  "Game.hpp"

class Map {
public:
    Map();
    ~Map();

    void LoadMap(int arr[20][25]);
    void DrawMap();


private:
    SDL_FRect srcR,destR;

    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    int map[20][25];

};

#endif //INC_2DP_MAP_H