#pragma once

#include <triton/app.hpp>
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
        Stopwatch stopwatch;

        static void OnRender(EventArgs<App, FPSController, App::RenderEvent>* event);

        void OnEnable() override;
        void OnDisable() override;
    public:
        int GetFPS();
        void SetCap(int cap);
    };
}
