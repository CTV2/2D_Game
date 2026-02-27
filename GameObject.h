// Declares a basic textured object with update and render behavior.
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
    void Height_Width(float W, float H);
private:
    float x_pos;
    float y_pos;
    float width_mod;
    float height_mod;

    SDL_Texture* objTexture;
    SDL_FRect srcR,destR;


};
#endif //INC_2DP_GAMEOBJECT_H