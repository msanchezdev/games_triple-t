#include <SDL2/SDL.h>
#include <triton/app.hpp>
#include "sprite.hpp"

using namespace triton;

void Sprite_OnRender(EventArgs<App, Sprite, App::RenderEvent>* event) {
    event->reference->Render();
}

Sprite::Sprite(SDL_Surface* surface)
    : Sprite(surface, new Vector { 0, 0 }) {
}

Sprite::Sprite(SDL_Surface* surface, Vector* position)
    : Sprite(surface, position, new Size(surface->w, surface->h)) {
}

Sprite::Sprite(SDL_Surface* surface, Vector* position, Size* size)
    : Sprite(surface, new Rect(position, size)) {
}

Sprite::Sprite(SDL_Surface* surface, Rect* rect) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->rect = *rect;
    this->SetOpacity(1.0);
}

Sprite::~Sprite() {
    SDL_DestroyTexture(this->texture);
}

void Sprite::Render() {
    if (!visible) {
        return;
    }

    SDL_Rect rect = (SDL_Rect)this->rect;
    SDL_RenderCopy(app.renderer, this->texture, nullptr, &rect);
}

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

Sprite* Sprite::SetOpacity(double opacity) {
    this->opacity = 255 * opacity;
    SDL_SetTextureAlphaMod(this->texture, this->opacity);

    return this;
}

void Sprite::OnAttach(GameObject* game_object) {
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(Sprite_OnRender, this));
}

void Sprite::OnDetach(GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(Sprite_OnRender, this));
}
