#pragma once

#include <SDL2/SDL_stdinc.h>
#include <triton/app.hpp>
#include <triton/types/game_object.hpp>
#include "fps_controller.hpp"

using namespace triton;

void FPSController::OnEnable() {
    stopwatch.Start();
    app.events.Subscribe(App::EventType::Render, new App::RenderEventHandler(FPSController::OnRender, this));
}

void FPSController::OnDisable() {
    stopwatch.Stop();
    app.events.Unsubscribe(App::EventType::Render, new App::RenderEventHandler(FPSController::OnRender, this));
}

void FPSController::OnRender(EventArgs<App, FPSController, App::RenderEvent>* event) {
    auto counter = event->reference;
    int current_frame = SDL_GetTicks();

    counter->delta_time = current_frame - counter->prev_frame;
    counter->prev_frame = current_frame;
    counter->frames++;

    if (!counter->stopwatch.GetElapsedTime()) {
        counter->fps = 0;
    } else {
        int elapsedTime = ceil(counter->stopwatch.GetElapsedTime() / 1000.0f);
        counter->fps = counter->frames / elapsedTime;
    }

    // cap the FPS
    if (counter->fps_cap) {
        int delay = counter->ticks_per_frame - counter->delta_time;
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
}

int FPSController::GetFPS() {
    return this->fps;
}

void FPSController::SetCap(int cap) {
    this->fps_cap = cap;
    this->ticks_per_frame = cap ? ceil(1000.0f / cap) : 0;
}
