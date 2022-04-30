#include <SDL2/SDL_log.h>
#include "core.hpp"

int main(int argc, char** argv) {
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);

    TT_Initialize();
    TT_Debug_PrintInitializedSubsystems();

    TT_EventLoop();
    TT_Shutdown();
    return 0;
}
