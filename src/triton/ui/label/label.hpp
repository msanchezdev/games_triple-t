#pragma once

/**
 * TODO: Add line height control
 */

#include <string>
#include <triton/app.hpp>
#include <triton/events.hpp>
#include <triton/types/game_object.hpp>
#include <triton/types.hpp>
#include <SDL2/SDL_ttf.h>

using namespace std;
using namespace triton;

namespace triton::ui {
    class LabelStyle {
    public:
        enum class EventType {
            StyleChanged,
            FontFamilyChanged,
            FontSizeChanged,
            ColorChanged,
            BackgroundColorChanged
        };

        typedef struct {
            EventType type;
        } StyleChangedEvent;
        DefineEventHandler(StyleChangedEventHandler, LabelStyle, StyleChangedEvent);

    private:
        EventListener<LabelStyle, EventType>* events = new EventListener<LabelStyle, EventType>(this);
        string font_family = "";
        int font_size = 16;
        SDL_Color color = { 0x00, 0x00, 0x00, 0xFF };
        SDL_Color background_color = { 0xFF, 0xFF, 0xFF, 0xFF };

        friend class Label;
    public:
        LabelStyle* SetFontFamily(string font_family);
        string GetFontFamily();

        LabelStyle* SetFontSize(int font_size);
        int GetFontSize();

        LabelStyle* SetColor(SDL_Color color);
        SDL_Color GetColor();

        LabelStyle* SetBackgroundColor(SDL_Color background_color);
        SDL_Color GetBackgroundColor();
    };

    class Label : public Component {
    private:
        string text;
        Rect rect;
        bool visible = false;
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;
        LabelStyle* style = nullptr;
        bool autosize = false;

        // Need to pre-render the label every time a property is changed.
        void Prerender();
        static void OnStyleChanged(EventArgs<LabelStyle, Label, LabelStyle::StyleChangedEvent>* event);
        static void OnRender(EventArgs<App, Label, App::RenderEvent>* event);

        void OnEnable() override;
        void OnDisable() override;

    public:
        Label();
        Label(LabelStyle* style = nullptr);
        Label(string text, LabelStyle* style = nullptr);
        Label(string text, Vector* position, LabelStyle* style = nullptr);
        Label(string text, Vector* position, Size* size, LabelStyle* style = nullptr);
        Label(string text, Rect* rect, LabelStyle* style = nullptr);
        ~Label();


        Label* SetText(string text);
        string GetText();

        Label* SetAutosize(bool autosize);
        bool GetAutosize();

        Label* SetPosition(Vector* position);
        Vector* GetPosition();

        Label* SetVisible(bool visible);
        bool IsVisible();

        LabelStyle* GetStyle();
    };
}
