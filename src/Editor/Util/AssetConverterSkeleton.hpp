#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>
#include <Engine/Animation/AnimationClip.hpp>

/// Class used to convert fbx to .asset format for skeleton and animation.
class AssetConverterSkeleton {
    public:
        /// Constructor.
        AssetConverterSkeleton();

        /// Destructor.
        ~AssetConverterSkeleton();

        /// Convert a mesh from fbx to .asset format.
        /**
         * @param filepath Filepath of the fbxfile.
         * @param destination Filepath of the destination inculding name and extension.
         * @param bindpose Is it the skeleton of the animation?
         */
        bool Convert(const char* filepath, const char* destination,
            bool bindpose);
    
        /// Check after conversion if everything went well.
        /**
         * @return True if successfully converted model.
         */
        bool Success() const;
    
        /// String with errors and warnings.
        /**
         * @return A string with messages of what went wrong durning conversion.
         */
        const std::string& GetErrorString();
    
    private:
        void BoneRecursive(aiNode* node, int parent);

        Assimp::Importer aImporter;

        std::vector<std::string> bones;
        std::vector<std::string> children;
        std::vector<int> parents;
        std::vector<int> ids;
        unsigned int currentId = 0;
        
        bool success = true;
        std::string errorString;
};