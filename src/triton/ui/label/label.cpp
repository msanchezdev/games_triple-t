#include <SDL2/SDL_ttf.h>
#include <triton/app.hpp>
#include <triton/types.hpp>
#include "label.hpp"
#include "../../../../games/triple-t/resources.hpp"

using namespace triton;
using namespace triton::ui;

Label::Label(string text, LabelStyle* style)
    : Label(text, new Vector(), style) {
}

Label::Label(string text, Vector* position, LabelStyle* style)
    : Label(text, position, new Size(), style) {
}

Label::Label(string text, Vector* position, Size* size, LabelStyle* style)
    : Label(text, new Rect(position, size), style) {
}

Label::Label(string text, Rect* rect, LabelStyle* style) {
    this->rect = *rect;
    this->text = text;

    this->style = style == nullptr ? new LabelStyle {
        .font_name = RES_FONT_SHADOWSINTOLIGHT,
        .font_size = 16,
        .color = { 0x00, 0x00, 0x00, 0xFF },
        .background_color = { 0xFF, 0xFF, 0xFF, 0xFF }
    } : style;
}

void Label::OnAttach(GameObject* game_object) {
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(Label::OnRender, this));
}

void Label::OnDetach(GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(Label::OnRender, this));
}

void Label::OnRender(EventArgs<App, Label, App::RenderEvent>* event) {
    Label* label = event->reference;
    FontResource* font = app.fonts[label->style->font_name];

    // TODO: Change font properties based on style
    TTF_SetFontSize(font->font, label->style->font_size);


    SDL_Surface* label_surface = TTF_RenderText_Blended(font->font, label->text.c_str(), label->style->color);
    SDL_Texture* label_texture = SDL_CreateTextureFromSurface(app.renderer, label_surface);
    SDL_RenderCopy(app.renderer, label_texture, nullptr, new SDL_Rect {
        label->rect.x, label->rect.y,
        label_surface->w, label_surface->h,
    });

    SDL_DestroyTexture(label_texture);
    SDL_FreeSurface(label_surface);
}
