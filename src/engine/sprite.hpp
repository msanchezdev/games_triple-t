#pragma once
#include "SDL2/SDL_render.h"
#include "types.hpp"

class TRT_Sprite {
public:
    TRT_Point2D position;
    TRT_Size2D size;
    SDL_Texture* texture;


    TRT_Sprite(SDL_Surface* surface);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D position);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D position, TRT_Size2D size);
    ~TRT_Sprite();

    void Render();
    TRT_Sprite* Register();
    TRT_Sprite* Unregister();
};
