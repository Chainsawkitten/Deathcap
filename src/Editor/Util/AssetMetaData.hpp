#pragma once

#include <ctime>
#include <json/json.h>

/// Static class for metadata generation.
/**
 * A metadata file contains information about a file.
 * The extension for a meta file is .meta.
 * Current metadata support exists for:
 * Meshes
 * 
 * @todo To be implemented:
 * Materials
 * Animations
 * Skeletons
 */
class AssetMetaData
{
    public:
        /// Metadata for mesh asset.
        struct MeshImportData {
            bool triangulate;
            bool importNormals;
            bool importTangents;
            bool CPU;
            bool GPU;
        };

        //TMPTODO
        /// Generate metadata for a mesh.
        /**
         * @param filepath Filepath with extensions.
         * @param metaData Metadata to be exported.
         * @return Whether the file could be opened.
         */
        static bool SaveMetaData(const char* filepath, MeshImportData* metaData);

        //TMPTODO
        /// Get metadata from a mesh.
        /**
         * @param filepath Filepath with extensions.
         * @return Metadata pointer with info about the file. You will have to delete it yourself.
         */
        static bool LoadMetaData(const char* filepath, MeshImportData* metaData);

        //TMPTODO
        static void ToJson(const MeshImportData* metaData, Json::Value& node);

        //TMPTODO
        static void FromJson(const Json::Value& node, MeshImportData* metaData);

        //TMPTODO
        static bool AssetMetaData::Save(const char* filepath, Json::Value& node);

        //TMPTODO
        static bool AssetMetaData::Load(const char* filepath, Json::Value& node);

    protected:
        /// Constructor.
        AssetMetaData() {}

        /// Destructor.
        ~AssetMetaData() {}
};
