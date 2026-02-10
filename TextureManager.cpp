#include "TextureManager.h"


SDL_Texture *TextureManager::LoadTexture(const char* texture) {
        SDL_Surface* tmp_surface = IMG_Load(texture);
        if (!tmp_surface) {
            SDL_Log("IMG_Load failed for '%s': %s", texture, SDL_GetError());
            return nullptr;
        }
        SDL_Texture* tex  = SDL_CreateTextureFromSurface(Game::renderer,tmp_surface);
        SDL_DestroySurface(tmp_surface);
        if (!tex) {
            SDL_Log("SDL_CreateTextureFromSurface failed for '%s': %s", texture, SDL_GetError());
            return nullptr;
        }
        return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_FRect srcR, SDL_FRect destR) {
     SDL_RenderTexture(Game::renderer,tex ,&srcR,&destR);
}
