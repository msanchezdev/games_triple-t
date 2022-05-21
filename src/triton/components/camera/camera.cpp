#include <triton/app.hpp>
#include "camera.hpp"

using namespace triton;

Camera::Camera() {
    this->size = { 1, 1 };
    this->SetViewport({ 0, 0, app.resolution.width, app.resolution.height });
    this->SetSize({ app.resolution.width / 1units, app.resolution.height / 1units });
}

void Camera::OnEnable() {
    app.events.Subscribe(App::EventType::CameraRender, new App::CameraRenderEventHandler(Camera::OnCameraRender, this));
}

void Camera::OnDisable() {
    app.events.Unsubscribe(App::EventType::CameraRender, new App::CameraRenderEventHandler(Camera::OnCameraRender, this));
}

void Camera::OnCameraRender(EventArgs<App, Camera, App::RenderEvent>* event) {
    auto camera = event->reference;
    SDL_SetRenderDrawColor(app.renderer, camera->clear_color.r, camera->clear_color.g, camera->clear_color.b, camera->clear_color.a);
    SDL_RenderFillRectF(app.renderer, (SDL_FRect*)&camera->viewport);

    if (!camera->is_grid_visible) {
        return;
    }

    // draw half lines
    SDL_SetRenderDrawColor(app.renderer, 0xDB, 0xDB, 0xDB, 0xFF);
    for (float y = 0.5; y < camera->size.height; y++) {
        float x1 = 0;
        float y1 = y;
        float x2 = camera->size.width;
        float y2 = y;

        camera->DrawLine(x1, y1, x2, y2);
    }

    for (float x = 0.5; x < camera->size.width; x++) {
        float x1 = x;
        float y1 = 0;
        float x2 = x;
        float y2 = camera->size.height;

        camera->DrawLine(x1, y1, x2, y2);
    }

    // draw lines
    SDL_SetRenderDrawColor(app.renderer, 0x3C, 0x3C, 0x3C, 0xFF);
    for (float y = 0; y < camera->size.height; y++) {
        float x1 = 0;
        float y1 = y;
        float x2 = camera->size.width;
        float y2 = y;

        camera->DrawLine(x1, y1, x2, y2);
    }

    for (float x = 0; x < camera->size.width; x++) {
        float x1 = x;
        float y1 = 0;
        float x2 = x;
        float y2 = camera->size.height;

        camera->DrawLine(x1, y1, x2, y2);
    }
}

Camera* Camera::SetSize(Size size) {
    this->size = size;
    this->scale = Vector(viewport.width / (this->size.width * 1units), viewport.height / (this->size.height * 1units));
    return this;
}

Size Camera::GetSize() {
    return this->size;
}

Camera* Camera::SetGridVisible(bool visible) {
    this->is_grid_visible = visible;
    return this;
}

bool Camera::IsGridVisible() {
    return this->is_grid_visible;
}

Camera* Camera::SetClearColor(Color color) {
    this->clear_color = color;
    return this;
}

Color Camera::GetClearColor() {
    return this->clear_color;
}

Camera* Camera::DrawLine(float x1, float y1, float x2, float y2) {
    x1 = this->viewport.x + x1 * this->scale.x * 1units;
    y1 = this->viewport.y + y1 * this->scale.y * 1units;
    x2 = this->viewport.x + x2 * this->scale.x * 1units;
    y2 = this->viewport.y + y2 * this->scale.y * 1units;

    SDL_RenderDrawLineF(app.renderer, x1, y1, x2, y2);
    return this;
}

Camera* Camera::DrawLine(Vector from, Vector to) {
    return this->DrawLine(from.x, from.y, to.x, to.y);
}

Camera* Camera::SetViewport(Rect viewport) {
    this->viewport = viewport;
    this->scale = Vector(viewport.width / (this->size.width * 1units), viewport.height / (this->size.height * 1units));
    return this;
}

Rect Camera::GetViewport() {
    return this->viewport;
}
