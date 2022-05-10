/**
 * UI Labels are used to display text.
 */

#pragma once

#include <string>
#include <triton/app.hpp>
#include <triton/events.hpp>
#include <triton/game_object.hpp>
#include <triton/types.hpp>
#include <SDL2/SDL_ttf.h>

using namespace std;
using namespace triton;

namespace triton::ui {
    typedef struct LabelStyle {
        string font_name;
        int font_size;
        SDL_Color color;
        SDL_Color background_color;
    } LabelStyle;

    class Label : public Component {
    private:
        string text;
        Rect rect;

        static void OnRender(EventArgs<App, Label, App::RenderEvent>* event);

    public:
        LabelStyle* style = nullptr;

        Label(string text, LabelStyle* style = nullptr);
        Label(string text, Vector* position, LabelStyle* style = nullptr);
        Label(string text, Vector* position, Size* size, LabelStyle* style = nullptr);
        Label(string text, Rect* rect, LabelStyle* style = nullptr);

        void SetText(string text);
        string GetText();

        void SetPosition(Vector* position);
        Vector* GetPosition();

        void OnAttach(GameObject* game_object) override;
        void OnDetach(GameObject* game_object) override;
    };
}
