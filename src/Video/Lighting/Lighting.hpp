#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

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
    };
}
