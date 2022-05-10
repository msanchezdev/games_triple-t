#pragma once

#include <SDL2/SDL_render.h>
#include <triton/game_object.hpp>
#include <triton/types.hpp>

namespace triton {
    class Sprite : public Component {
    private:
        SDL_Surface* surface;
        Uint8 opacity = 255;

    public:
        Rect rect;
        SDL_Texture* texture;
        bool visible = true;

        Sprite(SDL_Surface* surface);
        Sprite(SDL_Surface* surface, Rect* rect);
        Sprite(SDL_Surface* surface, Vector* position);
        Sprite(SDL_Surface* surface, Vector* position, Size* size);
        ~Sprite();

        void Render();
        Sprite* SetImage(SDL_Surface* surface);
        SDL_Surface* GetImage();

        Sprite* SetVisible(bool visible);
        Sprite* SetOpacity(double opacity);

        void OnAttach(GameObject* game_object) override;
        void OnDetach(GameObject* game_object) override;
    };
}
