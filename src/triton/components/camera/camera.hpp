#pragma once

/**
 * Camera component
 */

#include <triton/types.hpp>
#include <triton/app.hpp>

namespace triton {
    /**
     * The camera is used to render
     */
    class Camera : public Component {
    private:
        Size size;
        Rect viewport;
        Vector scale;

        Color clear_color = { 0xA1, 0xA1, 0xA1, 0xFF };
        bool is_grid_visible = false;

        void OnEnable() override;
        void OnDisable() override;

        static void OnCameraRender(EventArgs<App, Camera, App::RenderEvent>* event);
        friend void App::Render();
    public:
        Camera();

        /**
         * Change the camera size (how many units the camera can see)
         */
        Camera* SetSize(Size size);

        /**
         * Get the camera size (how many units the camera can see)
         */
        Size GetSize();

        /**
         * Change the camera viewport (where the camera is rendered, in pixels)
         */
        Camera* SetViewport(Rect viewport);

        /**
         * Get the camera viewport (where the camera is rendered, in pixels)
         */
        Rect GetViewport();

        /**
         * DEBUG: Set the grid visibility.
         */
        Camera* SetGridVisible(bool visible);

        /**
         * DEBUG: Defines if a grid should be drawn in the camera's viewport.
         */
        bool IsGridVisible();

        /**
         * Set the clear color of the camera.
         */
        Camera* SetClearColor(Color color);

        /**
         * Get the clear color of the camera.
         */
        Color GetClearColor();

        Camera* DrawLine(float x1, float y1, float x2, float y2);
        Camera* DrawLine(Vector from, Vector to);
    };
}
