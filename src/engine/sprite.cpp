#include <SDL2/SDL.h>
#include "app.hpp"
#include "sprite.hpp"

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
}

void TRT_Sprite::Render() {
    SDL_Rect rect = {
        .x = position.x,
        .y = position.y,
        .w = size.width,
        .h = size.height
    };

    SDL_RenderCopy(app.renderer, this->texture, NULL, &rect);
}

void TRT_Sprite_Render(void* sprite) {
    TRT_Sprite* s = (TRT_Sprite*)sprite;
    s->Render();
}

TRT_Sprite* TRT_Sprite::Register() {
    app.events->Subscribe(this, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(TRT_Sprite_Render));

    return this;
}

TRT_Sprite* TRT_Sprite::Unregister() {
    app.events->Unsubscribe(this, TRT_EventType::EVENT_RENDER, new TRT_RenderEventHandler(TRT_Sprite_Render));

    return this;
}
