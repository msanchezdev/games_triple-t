/**
 * TODO: Add line height control
 */
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
    // autosize will be enabled automatically unless a size is provided
    this->autosize = true;
}

Label::Label(string text, Vector* position, Size* size, LabelStyle* style)
    : Label(text, new Rect(position, size), style) {
}

Label::Label(string text, Rect* rect, LabelStyle* style) {
    this->rect = *rect;
    this->text = text;
    this->visible = true;

    this->style = style == nullptr ? new LabelStyle() : style;
    if (!style) {
        this->style->SetFontFamily(RES_FONT_SHADOWSINTOLIGHT);
        this->style->SetFontSize(16);
        this->style->SetColor({ 0x00, 0x00, 0x00, 0xFF });
        this->style->SetBackgroundColor({ 0xFF, 0xFF, 0xFF, 0x00 });
        this->style->events->Subscribe(
            LabelStyle::EventType::StyleChanged,
            new LabelStyle::StyleChangedEventHandler(Label::OnStyleChanged, this)
        );
    }
    this->Prerender();
}

Label::~Label() {
    SDL_DestroyTexture(this->texture);
    SDL_FreeSurface(this->surface);
}

void Label::OnEnable(GameObject* game_object) {
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(Label::OnRender, this));
}

void Label::OnDisable(GameObject* game_object) {
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(Label::OnRender, this));
}

void Label::Prerender() {
    if (this->texture != nullptr) {
        SDL_DestroyTexture(this->texture);
    }

    if (this->surface != nullptr) {
        SDL_FreeSurface(this->surface);
    }

    TTF_Font* font = app.fonts[style->GetFontFamily()]->font;
    TTF_SetFontSize(font, style->GetFontSize());

    if (this->autosize) {
        surface = TTF_RenderText_Shaded(font, text.c_str(), style->GetColor(), style->GetBackgroundColor());
        this->rect.width = surface->w;
        this->rect.height = surface->h;
    } else {
        surface = TTF_RenderText_Shaded_Wrapped(font, text.c_str(), style->GetColor(), style->GetBackgroundColor(), this->rect.width);
    }
    texture = SDL_CreateTextureFromSurface(app.renderer, surface);
}

void Label::OnRender(EventArgs<App, Label, App::RenderEvent>* event) {
    Label* label = event->reference;

    if (!label->visible) {
        return;
    }

    SDL_Rect clip = {
        0, 0,
        label->rect.width, label->rect.height
    };
    SDL_RenderCopy(app.renderer, label->texture, &clip, (SDL_Rect*)&label->rect);
}

void Label::OnStyleChanged(EventArgs<LabelStyle, Label, LabelStyle::StyleChangedEvent>* event) {
    event->reference->Prerender();
}

Label* Label::SetText(string text) {
    this->text = text;
    this->Prerender();
    return this;
}

string Label::GetText() {
    return this->text;
}

Label* Label::SetAutosize(bool autosize) {
    this->autosize = autosize;
    this->Prerender();
    return this;
}
bool Label::GetAutosize() {
    return this->autosize;
}

Label* Label::SetPosition(Vector* position) {
    this->rect.x = position->x;
    this->rect.y = position->y;
    return this;
}

Vector* Label::GetPosition() {
    return this->rect.GetVector();
}

Label* Label::SetVisible(bool visible) {
    this->visible = visible;
    return this;
}

bool Label::IsVisible() {
    return this->visible;
}

LabelStyle* Label::GetStyle() {
    return this->style;
}

// -------- Label Style --------
LabelStyle* LabelStyle::SetFontFamily(string font_family) {
    this->font_family = font_family;

    LabelStyle::StyleChangedEvent event {
        .type = LabelStyle::EventType::FontFamilyChanged
    };

    this->events->Emit(LabelStyle::EventType::StyleChanged, &event);
    this->events->Emit(LabelStyle::EventType::FontFamilyChanged, &event);
    return this;
}

string LabelStyle::GetFontFamily() {
    return this->font_family;
}

LabelStyle* LabelStyle::SetFontSize(int font_size) {
    this->font_size = font_size;

    LabelStyle::StyleChangedEvent event {
        .type = LabelStyle::EventType::FontSizeChanged
    };

    this->events->Emit(LabelStyle::EventType::StyleChanged, &event);
    this->events->Emit(LabelStyle::EventType::FontSizeChanged, &event);
    return this;
}

int LabelStyle::GetFontSize() {
    return this->font_size;
}

LabelStyle* LabelStyle::SetColor(SDL_Color color) {
    this->color = color;

    LabelStyle::StyleChangedEvent event {
        .type = LabelStyle::EventType::ColorChanged
    };

    this->events->Emit(LabelStyle::EventType::StyleChanged, &event);
    this->events->Emit(LabelStyle::EventType::ColorChanged, &event);
    return this;
}

SDL_Color LabelStyle::GetColor() {
    return this->color;
}

LabelStyle* LabelStyle::SetBackgroundColor(SDL_Color color) {
    this->background_color = color;

    LabelStyle::StyleChangedEvent event {
        .type = LabelStyle::EventType::BackgroundColorChanged
    };

    this->events->Emit(LabelStyle::EventType::StyleChanged, &event);
    this->events->Emit(LabelStyle::EventType::BackgroundColorChanged, &event);
    return this;
}

SDL_Color LabelStyle::GetBackgroundColor() {
    return this->background_color;
}
