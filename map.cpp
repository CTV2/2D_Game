#include "map.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <vector>
#include <iostream>


int base[20][25] = {
    {2,1,1,1,0,1,0,1,1,1,0,1,0,1,0,0,2,2,2,2,2,1,2,2,2},
{2,0,0,2,2,2,0,2,0,1,1,2,0,2,1,0,1,0,0,1,1,2,1,2,2},
{0,0,1,0,1,0,2,2,0,1,1,0,0,0,0,1,0,1,0,2,1,1,2,2,1},
{1,1,0,2,0,0,2,1,1,2,1,1,0,0,1,1,1,1,1,2,2,1,1,2,2},
{1,1,1,0,1,1,2,2,0,1,1,1,2,1,1,1,0,2,0,0,2,1,1,1,1},
{0,2,1,2,2,0,2,2,1,2,0,2,0,0,2,2,1,2,2,1,0,0,1,2,2},
{1,0,2,1,1,0,0,1,1,0,0,0,1,1,2,2,0,0,2,1,1,0,0,0,1},
{1,0,0,2,2,2,2,0,2,0,1,1,0,2,1,1,0,1,1,1,2,1,1,0,0},
{1,0,1,1,2,0,0,2,1,1,0,1,2,0,2,0,2,2,1,1,2,2,1,1,1},
{2,1,2,0,1,1,1,0,1,2,2,2,0,1,2,0,0,1,2,1,1,2,2,2,1},
{2,1,1,0,1,0,0,0,2,0,2,0,0,1,2,1,1,2,2,2,1,1,2,2,2},
{1,1,2,0,1,1,2,2,0,0,0,2,1,1,0,1,1,1,0,2,2,2,2,2,2},
{1,1,1,1,2,2,0,1,2,1,2,1,2,2,1,2,2,1,2,1,1,2,2,1,2},
{1,1,2,2,2,0,0,0,0,1,1,0,0,1,1,1,1,2,2,1,1,1,2,1,1},
{1,1,2,0,0,0,1,2,1,2,0,2,1,0,2,0,0,2,2,1,1,1,1,1,2},
{0,0,2,1,0,1,1,1,0,2,2,2,2,1,0,0,2,0,2,2,2,1,1,2,2},
{1,1,2,2,0,2,2,0,1,1,1,2,0,1,1,0,0,0,2,2,2,2,2,2,2},
{2,1,1,0,1,2,1,0,0,0,2,0,2,1,1,2,2,1,0,1,1,1,1,2,2},
{0,0,2,2,0,0,1,0,0,1,0,2,2,1,0,0,2,0,2,2,1,1,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};



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

    // build_temp();
    // LoadMap(base);
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


// void Map::LoadMap(int arr[20][25]) {
//     for (int row = 0; row < 20; ++row) {
//         for (int col = 0; col < 25; ++col) {
//             map[row][col] = arr[row][col];
//         }
//     }
// }

// void Map::DrawMap() {

//     int type = 0;
//     for (int row = 0; row < 20; ++row) {
//         for (int col = 0; col < 25; ++col) {
//             type = map[row][col];

//             destR.x = col * 32;
//             destR.y = row * 32;

//             switch (type) {
//                 case 0:
//                     TextureManager::Draw(water,srcR,destR);
//                     break;
//                 case 1:
//                     TextureManager::Draw(grass,srcR,destR);
//                     break;
//                 case 2:
//                     TextureManager::Draw(dirt,srcR,destR);
//                     break;
//                 default:
//                     break;

//             }
//         }
//     }
// }

void Map::Draw_OBJ() {

    // Loops through vector of objects and renders each one
    for (int i = 0; i < (height*width); i++) {
        this->objects[i].Update();
        this->objects[i].Render();
    }
}



