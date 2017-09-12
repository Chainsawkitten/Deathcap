#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Video {
    /// Texture to write and read from.
    class ReadWriteTexture {
        public:
            /// Format types for read/write textures.
            enum FORMAT {
                RGB8 = 0, ///< Red, Green, Blue, 8 bits per channel.
                RGB16, ///< Red, Green, Blue, 16 bits per channel.
                DEPTH32, ///< Depth, 32 bits.
            };

            /// Create new read/write texture.
            /**
             * @param size Size of the new read/write texture.
             */
            ReadWriteTexture(const glm::vec2& size, FORMAT format);
            
            /// Destructor.
            ~ReadWriteTexture();

            /// Get size of texture.
            /**
             * @return The size of texture in pixels.
             */
            glm::vec2 GetSize() const;

            /// Get texture.
            /**
             * @return The texture.
             */
            GLuint GetTexture() const;

            /// Get format.
            /**
             * @return Format of texture.
             */
            FORMAT GetFormat() const;
            
        private:
            static void CreateTexture(GLuint& texture, unsigned int width, unsigned int height, GLenum format, GLint internalFormat);

            GLuint texture;

            glm::vec2 size;
            FORMAT format;
    };
}
