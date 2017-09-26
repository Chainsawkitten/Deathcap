#pragma once

#include <string>
#include <json/json.h>

namespace Video {
    class Texture2D;
}

/// A texture used in a hymn.
class TextureAsset {
    public:
        /// Create new texture asset.
        TextureAsset();
        
        TextureAsset(const TextureAsset & other);

        /// Destructor.
        ~TextureAsset();
        
        /// Save the texture asset.
        void Save() const;
        
        /// Load texture asset from disk.
        /**
         * @param name The name of the texture asset.
         */
        void Load(const std::string& name);
        
        /// Get the texture.
        /**
         * @return The texture.
         */
        Video::Texture2D* GetTexture() const;
        
        /// The name of the texture.
        std::string name;
        
        /// Whether to convert from SRGB space to linear space when loading.
        bool srgb = false;
        
    private:
        Video::Texture2D* texture;
};
