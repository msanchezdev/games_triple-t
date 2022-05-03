#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

class TRT_Sprite : public TRT_Component {
public:
    TRT_Point2D position;
    TRT_Size2D size;
    SDL_Texture* texture;
    bool visible = true;


    TRT_Sprite(SDL_Surface* surface);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D position);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D position, TRT_Size2D size);
    ~TRT_Sprite();

    void Render();
    void OnAttach(TRT_GameObject* game_object) override;
    void OnDetach(TRT_GameObject* game_object) override;
};
