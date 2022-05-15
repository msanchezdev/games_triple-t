#include <SDL2/SDL_timer.h>
#include "stopwatch.hpp"

using namespace triton;

void Stopwatch::Start() {
    this->start_time = SDL_GetTicks();
    this->running = true;
}

void Stopwatch::Stop() {
    this->end_time = SDL_GetTicks();
    this->elapsed_time = this->end_time - this->start_time;
    this->running = false;
}

int Stopwatch::GetElapsedTime() {
    if (this->running) {
        this->elapsed_time = SDL_GetTicks() - this->start_time;
    }

    return this->elapsed_time;
}

void Stopwatch::Reset() {
    this->start_time = SDL_GetTicks();
    this->elapsed_time = SDL_GetTicks();
    this->end_time = 0;
}
