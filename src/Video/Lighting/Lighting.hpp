#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Light.hpp"
#include <vector>

namespace Video {
    /// Holds the frame buffers used for deferred rendering.
    class Lighting {
        public:
            /// Create new deferred lighting.
            /**
             * @param screenSize Size of the screen in pixels.
             */
            Lighting(const glm::vec2& screenSize);
            
            /// Destructor.
            ~Lighting();
            
            /// Set as render target.
            void SetTarget();
            
            /// Clear all lights.
            void ClearLights();
            
            /// Add a light.
            /**
             * @param light The light to add.
             */
            void AddLight(const Light& light);
            
        private:
            enum TEXTURE_TYPE {
                DIFFUSE,
                NORMAL,
                SPECULAR,
                GLOW,
                NUM_TEXTURES
            };
            
            static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
            
            GLuint textures[NUM_TEXTURES];
            
            GLuint frameBufferObject;
            GLuint depthHandle;
            
            std::vector<Light> lights;
    };
}
