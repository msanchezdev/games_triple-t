#pragma once

#include <triton/app.hpp>
#include <triton/ui/label/label.hpp>
#include <triton/types/game_object.hpp>
#include <triton/stopwatch.hpp>

namespace triton {
    class FPSController : public Component {
    private:
        int frames = 0;
        int fps = 0;
        int fps_cap = 0;
        int prev_frame = 0;
        int delta_time = 0;
        int ticks_per_frame = 0;
        bool counter_visible = false;
        Stopwatch stopwatch;
        ui::Label counter = ui::Label("FPS: 0");

        static void OnRender(EventArgs<App, FPSController, App::RenderEvent>* event);

        void OnAttach() override;
        void OnDetach() override;

        void OnEnable() override;
        void OnDisable() override;

    public:
        int GetFPS();
        void SetCap(int cap);
        void SetCounterVisible(bool visible);
    };
}
