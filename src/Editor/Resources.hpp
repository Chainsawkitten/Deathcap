#pragma once

#include <vector>
#include <string>

class TextureAsset;
class ScriptFile;
namespace Geometry {
    class Model;
}
namespace Audio {
    class SoundBuffer;
}

/// A list of all the resources in a hymn.
class ResourceList {
    friend ResourceList& Resources();
    
    public:
        /// A resource.
        struct Resource {
            /// The type of resource.
            enum Type {
                SCENE = 0,
                MODEL,
                TEXTURE,
                SOUND
            } type;
            
            /// Scene name.
            std::string scene;
            
            /// Model.
            Geometry::Model* model;
            
            /// Texture.
            TextureAsset* texture;
            
            /// Sound.
            Audio::SoundBuffer* sound;
        };
        
        /// Save all resources to file.
        void Save() const;
        
        /// Load all resources from file.
        void Load();
        
        /// Clear resources.
        void Clear();
        
        /// Resources.
        std::vector<Resource> resources;
        
        /// The name of the activeScene.
        std::string activeScene;
        
        /// The id of the next model to create.
        unsigned int modelNumber = 0U;
        
        /// The id of the next texture to create.
        unsigned int textureNumber = 0U;
    
        /// The id of the next sound to create.
        unsigned int soundNumber = 0U;
        
    private:
        static ResourceList& GetInstance();
        
        ResourceList();
        ResourceList(ResourceList const&) = delete;
        void operator=(ResourceList const&) = delete;
};

/// Get the resource list.
/**
 * @return The %ResourceList instance.
 */
ResourceList& Resources();
