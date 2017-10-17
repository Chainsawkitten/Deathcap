#pragma once

#include <glm/glm.hpp>
#include "linking.hpp"

struct GLFWwindow;
class InputHandler;

/// The main game window in which the game is contained.
class MainWindow {
    public:
        /// Create new main window.
        /**
         * @param width Width of the window in pixels.
         * @param height Height of the window in pixels.
         * @param fullscreen Whether to start in fullscreen mode.
         * @param borderless Whether to use a borderless window.
         * @param title Window title.
         * @param debugContext Whether to activate OpenGL debug context.
         */
        ENGINE_EXPORT MainWindow(int width, int height, bool fullscreen = false, bool borderless = false, const char* title = "", bool debugContext = false);
        
        /// Destructor.
        ENGINE_EXPORT ~MainWindow();
        
        /// Get the instance of %MainWindow.
        /**
         * @return The %MainWindow instance.
         */
        ENGINE_EXPORT static MainWindow* GetInstance();
        
        /// Initialize components.
        /**
         * @param showNotifications Whether to show debug messages of notification priority.
         */
        ENGINE_EXPORT void Init(bool showNotifications = false) const;

        /// Update
        ENGINE_EXPORT void Update();
        
        /// Get the size of the window.
        /**
         * @return The size of the window in pixels.
         */
        ENGINE_EXPORT const glm::vec2& GetSize() const;

        /// Set the size of the window.
        /**
         * @param width The width of the window in pixels.
         * @param height The height of the window in pixels.
         */
        ENGINE_EXPORT void SetSize(int width, int height);

        /// Set window title.
        /**
         * @param title New window title.
         */
        ENGINE_EXPORT void SetTitle(const char* title) const;
        
        /// Get whether the window should close.
        /**
         * @return Whether the window should close
         */
        ENGINE_EXPORT bool ShouldClose() const;
        
        /// Close the window.
        ENGINE_EXPORT void Close();

        /// Cancel the closing of the window.
        ENGINE_EXPORT void CancelClose();
        
        /// Swap front- and backbuffers.
        ENGINE_EXPORT void SwapBuffers() const;
        
        /// Get GLFW window.
        /**
         * @return The GLFW window struct.
         */
        ENGINE_EXPORT GLFWwindow* GetGLFWWindow() const;
        
    private:
        // Copy Constructor
        MainWindow(MainWindow& mainwindow) = delete;

        static MainWindow* instance;
        
        InputHandler* input;
        GLFWwindow* window;
        bool debugContext;
        glm::vec2 size;
        
        bool shouldClose = false;
};
