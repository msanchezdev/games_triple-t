#include "defs.hpp"
#include "app.hpp"

App::App() {
    renderer = NULL;
    window = NULL;
    window_size = {
        .w = SCREEN_WIDTH,
        .h = SCREEN_HEIGHT
    };
}

App::~App() {
    // Unload images
    for (auto& image : images) {
        delete image.second;
    }
}

void App::LoadImage(const char* name, const char* path) {
    app.images[name] = new ImageResource(app.renderer, name, path);
}
