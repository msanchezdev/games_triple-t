#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

class TRT_Sprite : public TRT_Component {
private:
    SDL_Surface* surface;
    Uint8 opacity = 255;

public:
    TRT_Point2D* position;
    TRT_Size2D* size;
    TRT_Rect2D* rect;
    SDL_Texture* texture;
    bool visible = true;

    TRT_Sprite(SDL_Surface* surface);
    TRT_Sprite(SDL_Surface* surface, TRT_Rect2D* rect);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position);
    TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position, TRT_Size2D* size);
    ~TRT_Sprite();

    void Render();
    TRT_Sprite* SetImage(SDL_Surface* surface);
    SDL_Surface* GetImage();

    TRT_Sprite* SetVisible(bool visible);
    TRT_Sprite* SetOpacity(double opacity);

    void OnAttach(TRT_GameObject* game_object) override;
    void OnDetach(TRT_GameObject* game_object) override;
};
