#pragma once

#include <GL/glew.h>
#include <vector>
#include "ReadWriteTexture.hpp"

namespace Video {
    /// Frame buffer containing frame buffer object.
    class FrameBuffer {
        public:
            /// Create new frame buffer.
            /**
             * @param textures Vector of %ReadWriteTexture to create a frame buffer object.
             */
            FrameBuffer(const std::vector<ReadWriteTexture*>& textures);
            
            /// Destructor.
            ~FrameBuffer();

            /// Set as render target.
            void SetTarget() const;

        private:
            std::vector<ReadWriteTexture*> textures;

            GLuint frameBufferObject;
    };
}
