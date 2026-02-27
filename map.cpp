#include "map.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <vector>
#include <iostream>


// int base[20][25] = {
//     {2,1,1,1,0,1,0,1,1,1,0,1,0,1,0,0,2,2,2,2,2,1,2,2,2},
// {2,0,0,2,2,2,0,2,0,1,1,2,0,2,1,0,1,0,0,1,1,2,1,2,2},
// {0,0,1,0,1,0,2,2,0,1,1,0,0,0,0,1,0,1,0,2,1,1,2,2,1},
// {1,1,0,2,0,0,2,1,1,2,1,1,0,0,1,1,1,1,1,2,2,1,1,2,2},
// {1,1,1,0,1,1,2,2,0,1,1,1,2,1,1,1,0,2,0,0,2,1,1,1,1},
// {0,2,1,2,2,0,2,2,1,2,0,2,0,0,2,2,1,2,2,1,0,0,1,2,2},
// {1,0,2,1,1,0,0,1,1,0,0,0,1,1,2,2,0,0,2,1,1,0,0,0,1},
// {1,0,0,2,2,2,2,0,2,0,1,1,0,2,1,1,0,1,1,1,2,1,1,0,0},
// {1,0,1,1,2,0,0,2,1,1,0,1,2,0,2,0,2,2,1,1,2,2,1,1,1},
// {2,1,2,0,1,1,1,0,1,2,2,2,0,1,2,0,0,1,2,1,1,2,2,2,1},
// {2,1,1,0,1,0,0,0,2,0,2,0,0,1,2,1,1,2,2,2,1,1,2,2,2},
// {1,1,2,0,1,1,2,2,0,0,0,2,1,1,0,1,1,1,0,2,2,2,2,2,2},
// {1,1,1,1,2,2,0,1,2,1,2,1,2,2,1,2,2,1,2,1,1,2,2,1,2},
// {1,1,2,2,2,0,0,0,0,1,1,0,0,1,1,1,1,2,2,1,1,1,2,1,1},
// {1,1,2,0,0,0,1,2,1,2,0,2,1,0,2,0,0,2,2,1,1,1,1,1,2},
// {0,0,2,1,0,1,1,1,0,2,2,2,2,1,0,0,2,0,2,2,2,1,1,2,2},
// {1,1,2,2,0,2,2,0,1,1,1,2,0,1,1,0,0,0,2,2,2,2,2,2,2},
// {2,1,1,0,1,2,1,0,0,0,2,0,2,1,1,2,2,1,0,1,1,1,1,2,2},
// {0,0,2,2,0,0,1,0,0,1,0,2,2,1,0,0,2,0,2,2,1,1,1,1,1},
// {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
// };

// Makes map base of give size
Map::Map(int x, int y) {
    this->height = x;
    this->width = y;

    srcR.x = srcR.y = 0;
    srcR.w = destR.w = 32;
    srcR.h = destR.h = 32;
    destR.x = destR.y = 0;
}

// Make a map object based on 2D array, initializes textures
Map::Map(const std::vector<std::vector<int>>& grid) {

    // height and width of map
    this->height = grid.size();
    this->width = grid[0].size();

    std::cout << "Height: " << height << ", Width: " << width << std::endl;

    // Gives each map its own OBJ map
    this->objects.resize(height*width);


    // Updates OBJ map based on 2D array
    Init_obj(grid, this->objects);


    dirt = TextureManager::LoadTexture("textures/dirt.png");
    grass = TextureManager::LoadTexture("textures/grass.png");
    water = TextureManager::LoadTexture("textures/water.png");

    srcR.x = srcR.y = 0;
    srcR.w = destR.w = 32;
    srcR.h = destR.h = 32;

    destR.x = destR.y = 0;
}

// Takes in 2D array and vector of objects and initializes each object in vector to correct texture and position based on 2D array
void Map::Init_obj(const std::vector<std::vector<int>>& arr, std::vector<GameObject> &objects) {
    dirt = TextureManager::LoadTexture("textures/dirt.png");
    grass = TextureManager::LoadTexture("textures/grass.png");
    water = TextureManager::LoadTexture("textures/water.png");

    // Loops through Rows
    for (int i = 0; i < height; i++)
    {
        // Loops through Cols
        for (int j = 0; j < width; j++)
        {
            // Assigns each object in vector to correct texture and position based on 2D array
            if (arr[i][j] == 0){
                objects[i*25+j].ChangeTexturePreload(grass);
            }
            else if (arr[i][j] == 1){
                objects[i*25+j].ChangeTexturePreload(dirt);
            }
            else if (arr[i][j] == 2)
            {
                objects[i*25+j].ChangeTexturePreload(water);
            }
            objects[i*25+j].ChangePos(j*32,i*32);
        }
    }
}


void Map::Draw_OBJ() {

    // Loops through vector of objects and renders each one
    for (int i = 0; i < (height*width); i++) {
        this->objects[i].Update();
        this->objects[i].Render();
    }
}


// void Map::CreateMap_OBJ(string texture, float x, float y, ) {
//     GameObject obj(TextureManager::LoadTexture(texture.c_str()), x, y);
//     this->objects.push_back(obj);
// }
