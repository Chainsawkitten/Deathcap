#pragma once

#include <glm/glm.hpp>

namespace Video {
    struct Light;
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
            void AddLight(const Video::Light& light);
            
            /// Light the scene with the added lights.
            /**
             * @param inverseProjectionMatrix The camera's inverse projection matrix.
             */
            void Light(const glm::mat4& inverseProjectionMatrix);
            
            
        private:
            glm::vec2 screenSize;
            Lighting* lighting;
    };
}
