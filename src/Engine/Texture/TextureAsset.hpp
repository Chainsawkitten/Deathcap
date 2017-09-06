#pragma once

#include <string>
#include <json/json.h>

class Texture2D;

/// A texture used in a hymn.
class TextureAsset {
    public:
        /// Create new texture asset.
        TextureAsset();
        
        /// Destructor.
        ~TextureAsset();
        
        /// Save the texture.
        /**
         * @return JSON value to be stored on disk.
         */
        Json::Value Save() const;
        
        /// Load texture from JSON node.
        /**
         * @param node JSON node to load from.
         */
        void Load(const Json::Value& node);
        
        /// Get the texture.
        /**
         * @return The texture.
         */
        Texture2D* GetTexture() const;
        
        /// The name of the texture.
        std::string name;
        
        /// Whether to convert from SRGB space to linear space when loading.
        bool srgb = false;
        
    private:
        Texture2D* texture;
};
