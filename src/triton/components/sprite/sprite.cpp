#include <SDL2/SDL.h>
#include <triton/app.hpp>
#include "sprite.hpp"

void TRT_Sprite_OnRender(TRT_EventArgs<App, TRT_Sprite, App::RenderEvent>* event) {
    event->reference->Render();
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface)
    : TRT_Sprite(surface, new TRT_Point2D { 0, 0 }) {
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position)
    : TRT_Sprite(surface, position, new TRT_Size2D { surface->w, surface->h }) {
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position, TRT_Size2D* size)
    : TRT_Sprite(surface, new TRT_Rect2D { position, size }) {
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Rect2D* rect) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = rect->position;
    this->size = rect->size;
    this->rect = rect;
    this->SetOpacity(1.0);
}

TRT_Sprite::~TRT_Sprite() {
    SDL_DestroyTexture(this->texture);
}

void TRT_Sprite::Render() {
    if (!visible) {
        return;
    }

    SDL_Rect rect = {
        .x = position->x,
        .y = position->y,
        .w = size->width,
        .h = size->height
    };

    SDL_RenderCopy(app.renderer, this->texture, nullptr, &rect);
}

SDL_Surface* TRT_Sprite::GetImage() {
    return this->surface;
}

TRT_Sprite* TRT_Sprite::SetImage(SDL_Surface* surface) {
    SDL_DestroyTexture(this->texture);
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->SetOpacity(this->opacity);

    return this;
}

TRT_Sprite* TRT_Sprite::SetVisible(bool visible) {
    this->visible = visible;

    return this;
}

TRT_Sprite* TRT_Sprite::SetOpacity(double opacity) {
    this->opacity = 255 * opacity;
    SDL_SetTextureAlphaMod(this->texture, this->opacity);

    return this;
}

void TRT_Sprite::OnAttach(TRT_GameObject* game_object) {
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(TRT_Sprite_OnRender, this));
}

void TRT_Sprite::OnDetach(TRT_GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(TRT_Sprite_OnRender, this));
}
