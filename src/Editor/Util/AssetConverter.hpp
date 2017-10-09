#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Engine/Geometry/AssetFileHandler.hpp>
#include <Video/Geometry/VertexType/StaticVertex.hpp>
#include <Engine/Geometry/MathFunctions.hpp>


/// Struct to hold Material
/**
*
*/
struct MaterialData
{
    std::string type;
    std::string path;
};

/// Convert 3D file to a .asset file.
/**
 * Uses assimp to convert a 3D file to a .asset file.
 * Following 3D formats are tested:
 * FBX (.fbx)
 */
class AssetConverter
{
    public:
        /// Constructor.
        AssetConverter();

        /// Destructor.
        ~AssetConverter();

        /// Convert a mesh from fbx to .asset format.
        /**
         * @param filepath Filepath of the fbxfile.
         * @param destination Filepath of the destination inculding name and extension.
         * @param triangulate Should the mesh be triangulated?
         * @param importNormals Should normals be imported from the mesh?
         * @param importTangents Should tangents be imported from the mesh?
         */
        void Convert(const char * filepath, const char * destination,
            bool triangulate, bool importNormals, bool importTangents, bool importMaterial);

        /// Check after conversion if everything went well.
        /**
         * @return True if successfully converted model.
         */
        bool Success() const;
        
        /// String with errors and warnings.
        /**
         * @return A string with messages of what went wrong durning conversion.
         */
        std::string& GetErrorString();

        /// look for specifik textures connected to mesh
        /**
        * @ current material from scene
        * @ assimp holds different textertypes - _DIFFUSE, _NORMAL
        * @ type as string
        */
        std::vector<MaterialData> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    
    private:
        void ConvertMeshes(const aiScene * aScene, Geometry::AssetFileHandler * file);
        void ConvertMesh(aiMesh * mesh, Geometry::AssetFileHandler * file);
        Video::Geometry::VertexType::StaticVertex * ConvertStaticVertices(aiMesh * aMesh, Geometry::AssetFileHandler * file, unsigned int numVertices);
        Video::Geometry::VertexType::SkinVertex * ConvertSkinnedVertices(aiMesh * aMesh, Geometry::AssetFileHandler * file, unsigned int numVertices);
        void CalculateAABB(Geometry::AssetFileHandler::MeshData * meshData, unsigned int numVertices);

        std::vector<MaterialData> textures;

        Assimp::Importer aImporter;

        bool success = true;
        std::string errorString;
};
