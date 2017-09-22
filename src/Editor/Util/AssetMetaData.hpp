#pragma once

#include <fstream>
#include <ctime>

class AssetMetaData
{
    public:
        struct MeshImportData {
            char assetPath[128];
            char sourcePath[128];
            time_t saveTime;
            bool triangulate;
            bool importNormals;
            bool importTangents;
        };

        /**
         * @param filepath Filepath with extensions.
         * @param filepath Filepath with extensions.
         * @return False if file could not be opened.
         */
        static bool GenerateMetaData(const char * filepath, MeshImportData * metaData);

        /**
         * @param filepath Filepath with extensions.
         * @return Metadata, info about the file. You will have to delete it yourself.
         */
        static MeshImportData * GetMetaData(const char * filepath);

    protected:
        /// Constructor.
        AssetMetaData() {}

        /// Destructor.
        ~AssetMetaData() {}
};