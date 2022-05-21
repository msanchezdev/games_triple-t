#pragma once

#include <SDL2/SDL_render.h>
#include <triton/types/game_object.hpp>
#include <triton/types.hpp>

namespace triton {
    class Sprite : public Component {
    private:
        SDL_Surface* surface;
        SDL_Texture* texture;
        Uint8 opacity = 255;
        bool visible = true;

        void OnEnable() override;
        void OnDisable() override;

        static void OnCameraRender(EventArgs<App, Sprite, App::CameraRenderEvent>* event);
    public:

        Sprite(SDL_Surface* surface);
        ~Sprite();

        Sprite* SetImage(SDL_Surface* surface);
        SDL_Surface* GetImage();

        Sprite* SetVisible(bool visible);
        bool IsVisible();

        Sprite* SetOpacity(double opacity);
        double GetOpacity();
    };
}
