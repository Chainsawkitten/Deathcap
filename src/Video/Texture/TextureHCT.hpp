#pragma once

#include "Texture2D.hpp"
#include <cstdint>

namespace Video {
    /// Texture loaded from custom compressed texture format.
    /**
     * @todo DXT1 compression.
     * @todo Mipmaps.
     */
    class TextureHCT : public Texture2D {
        public:
        /// The type of compression the texture uses.
        enum CompressionType {
            BC1 = 0,
            BC4,
            BC5
        };
        
        /// Load texture.
        /**
         * @param filenameThe name of the HCT file to load.
         * @param textureReduction The mip-level to start loading.
         */
        VIDEO_API TextureHCT(const char* filename, uint16_t textureReduction);
        
        /// Destructor.
        VIDEO_API ~TextureHCT() override;
        
        /// Get OpenGL texture ID.
        /**
         * Used when binding a texture before draw calls.
         * @return The OpenGL texture identifier
         */
        VIDEO_API GLuint GetTextureID() const override;
        
        /// Get whether the texture has been loaded yet.
        /**
         * @return Whether the texture has been loaded yet.
         */
        VIDEO_API bool IsLoaded() const override;
        
        /// The version of the texture format.
        static const uint16_t VERSION = 3;
        
        private:
        GLuint texID = 0;
        bool loaded = false;
    };
}
