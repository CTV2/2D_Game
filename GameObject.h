#ifndef INC_2DP_GAMEOBJECT_H
#define INC_2DP_GAMEOBJECT_H
#include "Game.hpp"

class GameObject {
public:
    GameObject(const char* texturesheet, float x, float y);
    GameObject();
    ~GameObject();
    void Update();
    void Render();
    void ChangeTexturePreload(SDL_Texture* text);
    void ChangePos(float x, float y);
    void PrintPos();
private:
    float x_pos;
    float y_pos;

    SDL_Texture* objTexture;
    SDL_FRect srcR,destR;


};
#endif //INC_2DP_GAMEOBJECT_H