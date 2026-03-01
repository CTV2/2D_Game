// Declares static texture utility functions used by map and sprite rendering code.
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "SDL3/SDL.h"

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex,SDL_FRect srcR,SDL_FRect destR);
};

#endif
