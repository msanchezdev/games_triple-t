#pragma once

namespace triton {
    class GameObject;

    /**
     * Base class for creating custom components
     */
    class Component {
    private:
        GameObject* owner = nullptr;
        bool enabled = true;

        virtual void OnAttach();
        virtual void OnDetach();

        /**
         * Function called when the component is enabled.
         *
         * Override this function to perform any initialization and setup
         * that needs to be done every time the component is enabled.
         *
         * e.g. adding listeners to the `App::EventType::Render` event
         */
        virtual void OnEnable();

        /**
         * Function called when the component is disabled.
         *
         * Override this function to perform any cleanup and teardown
         * that needs to be done every time the component is disabled.
         *
         * e.g. removing listeners for the `App::EventType::Render` event
         */
        virtual void OnDisable();

        friend class GameObject;
    public:
        /**
         * Add this component to a game object
         *
         */
        void Attach(GameObject* game_object);

        /**
         * Remove this component from a game object.
         *
         * You need to take care of deleting the component yourself.
         */
        void Detach();

        /**
         * Get the owner GameObject
         */
        GameObject* GetOwner();

        /**
         * Check if this component is enabled
         */
        bool IsEnabled();

        /**
         * Enable this component
         */
        void Enable();

        /**
         * Disable this component
         */
        void Disable();
    };
}
