#pragma once

#include <GL/glew.h>
#include "../linking.hpp"

namespace Video {
    /// A two-dimensional texture.
    /**
     * Used to load a 2D texture from an image file.
     */
    class Texture2D {
        public:
            /// Create new unloaded texture.
            VIDEO_API Texture2D();
            
            /// Create new texture from given source string.
            /**
             * @param source Source string containing the image file.
             * @param sourceLength Length of the source string.
             */
            VIDEO_API Texture2D(const char* source, int sourceLength);
            
            /// Destructor
            VIDEO_API virtual ~Texture2D();
            
            /// Get OpenGL texture ID.
            /**
             * Used when binding a texture before draw calls.
             * @return The OpenGL texture identifier
             */
            VIDEO_API GLuint GetTextureID() const;
            
            /// Set how coordinates outside 0..1 are handled.
            /**
             * Default: GL_REPEAT
             * @param wrapMode One of GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER.
             */
            VIDEO_API void SetWrapping(GLint wrapMode);
            
            /// Load texture from file.
            /**
             * @param filename Filename (relative or absolute) of the image file.
             */
            VIDEO_API void Load(const char* filename);
            
            /// Load texture from memory.
            /**
             * @param source Source string containing the image file.
             * @param sourceLength Length of the source string.
             */
            VIDEO_API void Load(const char* source, int sourceLength);
            
            /// Get whether the texture has been loaded yet.
            /**
             * @return Whether the texture has been loaded yet.
             */
            VIDEO_API bool IsLoaded() const;
            
        protected:
            /// Get image GL format based on color components.
            /**
             * @param components The amount of components in the image. (1, 2, 3, or 4.)
             * @return One of GL_RED, GL_RG, GL_RGB or GL_RGBA
             */
            static GLenum Format(int components);
            
        private:
            Texture2D(const Texture2D & other) = delete;

            GLuint texID = 0;
            int width = 0;
            int height = 0;
            bool loaded = false;
    };
}
