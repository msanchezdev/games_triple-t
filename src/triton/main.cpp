#include <SDL2/SDL_log.h>
#include "core.hpp"

int main(int argc, char** argv) {
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

    TRT_Initialize();
    TRT_Debug_PrintInitializedSubsystems();

    TRT_EventLoop();
    TRT_Shutdown();
    return 0;
}
