// Declares static texture utility functions used by map and sprite rendering code.
#ifndef Texture_man
#define Texture_man
#include  "Game.hpp"

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static void Draw(SDL_Texture* tex,SDL_FRect srcR,SDL_FRect destR);
};

#endif
