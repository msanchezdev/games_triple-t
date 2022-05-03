#include "defs.hpp"
#include "app.hpp"
#include "utils.hpp"
#include <map>
#include <list>
#include <functional>

App::App() {}

App::~App() {}

void App::LoadImage(const char* name, const char* path) {
    app.images[name] = new ImageResource(app.renderer, name, path);
}

void App::Cleanup() {
    debug("Unloading images");
    for (auto& image : images) {
        delete image.second;
    }

    debug("Unregistering event handlers");
    delete events;

    debug("Destroying renderer");
    SDL_DestroyRenderer(app.renderer);

    debug("Closing main window");
    SDL_DestroyWindow(app.window);
}
