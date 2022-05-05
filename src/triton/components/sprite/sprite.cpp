#include <SDL2/SDL.h>
#include <triton/app.hpp>
#include "sprite.hpp"

void TRT_Sprite_OnRender(TRT_EventArgs<void>* sprite) {
    TRT_Sprite* sprite_component = sprite->sender->GetComponent<TRT_Sprite>();
    sprite_component->Render();
}

void TRT_Sprite::OnAttach(TRT_GameObject* game_object) {
    app.events->Subscribe(game_object, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(TRT_Sprite_OnRender));
}

void TRT_Sprite::OnDetach(TRT_GameObject* game_object) {
    app.events->Unsubscribe(game_object, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(TRT_Sprite_OnRender));
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = new TRT_Point2D { 0, 0 };
    this->size = new TRT_Size2D { surface->w, surface->h };
    this->rect = new TRT_Rect2D { this->position, this->size };
    this->SetOpacity(1.0);
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Rect2D* rect) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = rect->position;
    this->size = rect->size;
    this->rect = rect;
    this->SetOpacity(1.0);
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = position;
    this->size = new TRT_Size2D { surface->w, surface->h };
    this->rect = new TRT_Rect2D { this->position, this->size };
    this->SetOpacity(1.0);
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D* position, TRT_Size2D* size) {
    this->surface = surface;
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = position;
    this->size = size;
    this->rect = new TRT_Rect2D { this->position, this->size };
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
