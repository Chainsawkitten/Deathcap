#pragma once

#include <glm/glm.hpp>

namespace Video {
    class Light;
    class Lighting;
    
    /// Handles rendering using OpenGL.
    class Renderer {
        public:
            /// Create new renderer.
            /**
             * @param screenSize Size of the screen in pixels.
             */
            Renderer(const glm::vec2& screenSize);
            
            /// Destructor.
            ~Renderer();
            
            /// Clear the previous frame's data.
            void Clear();
            
            /// Start rendering the frame.
            void StartRendering();
            
            /// Add a light to the scene.
            void AddLight(const Light& light);
            
        private:
            glm::vec2 screenSize;
            Lighting* lighting;
    };
}
