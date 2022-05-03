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
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = { 0, 0 };
    this->size = { surface->w, surface->h };
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D position) {
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = position;
    this->size = { surface->w, surface->h };
}

TRT_Sprite::TRT_Sprite(SDL_Surface* surface, TRT_Point2D position, TRT_Size2D size) {
    this->texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    this->position = position;
    this->size = size;
}

TRT_Sprite::~TRT_Sprite() {
    SDL_DestroyTexture(this->texture);
    verbose("TRT_Sprite %p destroyed", this);
}

void TRT_Sprite::Render() {
    if (!visible) {
        return;
    }

    SDL_Rect rect = {
        .x = position.x,
        .y = position.y,
        .w = size.width,
        .h = size.height
    };

    SDL_RenderCopy(app.renderer, this->texture, nullptr, &rect);
}
