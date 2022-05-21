#include <SDL2/SDL.h>
#include <triton/app.hpp>
#include "sprite.hpp"

using namespace triton;

Sprite::Sprite(SDL_Surface* surface) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->SetOpacity(1.0);
}

Sprite::~Sprite() {
    SDL_DestroyTexture(this->texture);
}

void Sprite::OnEnable() {
    app.events.Subscribe(App::EventType::CameraRender, new App::CameraRenderEventHandler(Sprite::OnCameraRender, this));
}

void Sprite::OnDisable() {
    app.events.Unsubscribe(App::EventType::CameraRender, new App::CameraRenderEventHandler(Sprite::OnCameraRender, this));
}

void Sprite::OnCameraRender(EventArgs<App, Sprite, App::CameraRenderEvent>* event) {
    // SDL_FRect dst;
    // dst.x = 0;
    // dst.y = 0;
    // dst.w = 1;
    // dst.h = 1;

    // SDL_RenderCopy(app.renderer, event->reference->texture, nullptr, (SDL_Rect*)&dst);
}

// void Sprite::Render() {
//     if (!visible) {
//         return;
//     }

//     SDL_Rect rect = (SDL_Rect)this->rect;
// }

SDL_Surface* Sprite::GetImage() {
    return this->surface;
}

Sprite* Sprite::SetImage(SDL_Surface* surface) {
    SDL_DestroyTexture(this->texture);
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->SetOpacity(this->opacity);

    return this;
}

Sprite* Sprite::SetVisible(bool visible) {
    this->visible = visible;
    return this;
}

inline bool Sprite::IsVisible() {
    return this->visible;
}

Sprite* Sprite::SetOpacity(double opacity) {
    this->opacity = 255 * opacity;
    SDL_SetTextureAlphaMod(this->texture, this->opacity);

    return this;
}

double Sprite::GetOpacity() {
    return this->opacity / 255.0;
}
