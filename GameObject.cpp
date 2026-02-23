// Defines a renderable game object that loads a texture and updates its source/destination rectangles.
#include "GameObject.h"
#include "TextureManager.h"



GameObject::~GameObject() {

}

GameObject::GameObject(const char *texturesheet, float x, float y ) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    x_pos = x;
    y_pos = y;


}

GameObject::GameObject() {
    objTexture = NULL;
    x_pos = 0.0f;
    y_pos = 0.0f;
}


void GameObject::ChangeTexturePreload(SDL_Texture* text) {
    this->objTexture = text;
}

void GameObject::ChangePos(float x, float y) {
    this->x_pos = x;
    this->y_pos = y;
}
void GameObject::Update() {


    srcR.h = 32;
    srcR.w = 32;
    srcR.x = 50;
    srcR.y = 60;

    destR.x = x_pos;
    destR.y = y_pos;
    destR.w = srcR.w * 2;
    destR.h = srcR.h * 2;
}
void GameObject::PrintPos() {
    std::cout << "x_pos: " << x_pos << ", y_pos: " << y_pos << std::endl;
}


void GameObject::Render() {
    SDL_RenderTexture(Game::renderer,objTexture,NULL,&destR);
}
