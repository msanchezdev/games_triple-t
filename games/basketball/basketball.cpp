#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/engine.hpp>
#include <triton/components/fps_controller/fps_controller.hpp>
#include <triton/components/sprite/sprite.hpp>
#include "resources.hpp"

using namespace triton;

int main(int argc, char** argv) {
    app.Initialize();
    app.root.GetComponent<FPSController>()->SetCounterVisible(true);

    Sprite ball(
        app.images[RES_IMG_BALL]->surface,
        new Vector(
            (app.resolution.width / 2) - (app.images[RES_IMG_BALL]->surface->w / 2),
            app.resolution.height - app.images[RES_IMG_BALL]->surface->h - 5
        )
    );

    app.root.AddComponent(&ball);
    return app.Start();
}
