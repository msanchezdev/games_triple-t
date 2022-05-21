#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <triton/app.hpp>
#include <triton/types.hpp>
#include <triton/types/game_object.hpp>
#include <triton/components/camera/camera.hpp>
#include <triton/components/fps_controller/fps_controller.hpp>
#include <triton/components/sprite/sprite.hpp>
#include "resources.hpp"

using namespace triton;

int main(int argc, char** argv) {
    app.Initialize();
    app.root.GetComponent<FPSController>()->SetCounterVisible(true);

    // Ball
    GameObject ball = GameObject("ball");
    Sprite sprite = Sprite(app.images[RES_IMG_BALL]->surface);
    ball.AddComponent(&sprite);

    // Main Camera
    auto main_camera = app.CreateCamera("main_camera")
        ->SetGridVisible(true);

    // Minimap camera
    auto minimap_camera = app.CreateCamera("minimap")
        ->SetGridVisible(true)
        ->SetSize({ 4, 3 })
        ->SetClearColor(Color { 0xFF, 0x8A, 0x8A, 0xFF })
        ->SetViewport(Rect(app.resolution.width - 120, 20, 100, 100));

    info("Main camera at: %s", main_camera->GetOwner()->transform.position.ToString().c_str());
    info("Minimap camera at: %s", minimap_camera->GetOwner()->transform.position.ToString().c_str());
    return app.Start();
}
