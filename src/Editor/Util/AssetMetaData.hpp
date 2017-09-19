#pragma once

#include <fstream>

class AssetMetaData
{
    public:
        struct MeshImportData {
            bool triangulate;
            bool importNormals;
            bool importTangents;
        };

        /// Constructor.
        AssetMetaData();

        /// Destructor.
        ~AssetMetaData();

        /**
         * @param filepath Filepath with extensions.
         * @param filepath Filepath with extensions.
         * @return False if file could not be opened.
         */
        bool GenerateMetaData(const char * filepath, MeshImportData * metaData);

        /**
         * @param filepath Filepath with extensions.
         * @return Metadata, info about the file. You will have to delete it yourself.
         */
        MeshImportData * GetMetaData(const char * filepath);

    private:
        
};