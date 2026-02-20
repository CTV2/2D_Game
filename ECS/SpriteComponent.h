//
// Created by Rodney Brown on 2/20/26.
//

#ifndef INC_2DP_SPRITECOMPONENT_H_H
#define INC_2DP_SPRITECOMPONENT_H_H
#include "Component.h"
#include "SDL3/SDL.h"
#include "../TextureManager.h"
#include "PositionComponent.h"

class SpriteComponent : public Component {
    public:
        SpriteComponent() = default;
        SpriteComponent(const char* path) {
            setTexture(path);
        }

        void setTexture(const char* path) {
            texture = TextureManager::LoadTexture(path);
        }
        void init() override {
            position = &entity->getComponent<PositionComponent>();
            srcR = {0.0f, 0.0f, 32.0f, 32.0f};
            destR = {0.0f, 0.0f, 16.0f, 16.0f};
            if (!texture) {
                SDL_Log("SpriteComponent: texture was not loaded");
                return;
            }

            float texW = 0.0f;
            float texH = 0.0f;
            if (SDL_GetTextureSize(texture, &texW, &texH)) {
                srcR.w = texW;
                srcR.h = texH;
                const float scale = 0.20f;
                destR.w = texW * scale;
                destR.h = texH * scale;
            } else {
                SDL_Log("SDL_GetTextureSize failed: %s", SDL_GetError());
            }
        }
        void update() override {
            destR.x = position->x();
            destR.y = position->y();

        }
        void draw() override {
            TextureManager::Draw(texture,srcR,destR);

        }
    private:
    PositionComponent* position;
    SDL_Texture* texture;

    SDL_FRect srcR,destR;

};
#endif //INC_2DP_SPRITECOMPONENT_H_H
