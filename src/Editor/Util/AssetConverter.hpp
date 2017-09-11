#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Engine/Geometry/AssetFileHandler.hpp>
#include <Video/Geometry/VertexType/StaticVertex.hpp>
#include <Engine/Geometry/MathFunctions.hpp>

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
    	bool Convert(const char * filepath, const char * destination,
    		bool triangulate, bool importNormals,
    		bool importTangents);
    
    private:
    	void ConvertMeshes(const aiScene * aScene, Geometry::AssetFileHandler * file);
    	void ConvertMesh(aiMesh * mesh, Geometry::AssetFileHandler * file);
    
    	Assimp::Importer aImporter;
};