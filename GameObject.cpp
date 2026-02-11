#include "GameObject.h"
#include "TextureManager.h"


GameObject::~GameObject() {

}

GameObject::GameObject(const char *texturesheet, float x, float y ) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    x_pos = x;
    y_pos = y;

}

void GameObject::Update() {
     //x_pos++; slide
     //y_pos++; slide

    srcR.h = 32;
    srcR.w = 32;
    srcR.x = 50;
    srcR.y = 60;

    destR.x = x_pos;
    destR.y = y_pos;
    destR.w = srcR.w * 2;
    destR.h = srcR.h * 2;
}


void GameObject::Render() {
    SDL_RenderTexture(Game::renderer,objTexture,NULL,&destR);
}

