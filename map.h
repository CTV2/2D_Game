#ifndef INC_2DP_MAP_H
#define INC_2DP_MAP_H
#include "Game.hpp"
#include "GameObject.h"
#include <vector>



class Map {
public:
    Map(int x, int y);
    Map(const std::vector<std::vector<int>>& grid);
    ~Map();

    void LoadMap(int arr[20][25]);
    void DrawMap();
    void CreateMap_OBJ();
    void Init_obj(const std::vector<std::vector<int>>& arr, std::vector<GameObject>& objects);
    void Draw_OBJ();
    void Create_OBJ(const char* texturesheet, float x, float y);

    // 2D of Game object representing the map.
    std::vector<GameObject> objects;
    int height;
    int width;

private:
    SDL_FRect srcR,destR;

    SDL_Texture* dirt;
    SDL_Texture* grass;
    SDL_Texture* water;

    

};

#endif //INC_2DP_MAP_H