#pragma once

#include <fstream>
#include <cstdint>
#include <assert.h>
#include <vector>
#include <Video/Geometry/VertexType/StaticVertex.hpp>

namespace Geometry {
    /// Handler for .res format.
    /**
     * Start by using the Open() function. 
     * The Open() function requries a filepath and
     * a mode READ/WRITE.
     * End by using the Close() function.
     */
    class AssetFileHandler {
        public:
            /// Mode to use class. READ/WRITE.
            enum Mode {
        	    READ = 0,
        	    WRITE = 1
            };
        
            /// Mesh data.
            struct MeshData {
                uint32_t parent;
                uint32_t numVertices;
                uint32_t numIndices;
                glm::vec3 aabbDim;
                glm::vec3 aabbOrigin;
                glm::vec3 aabbMinpos;
                glm::vec3 aabbMaxpos;
                Video::Geometry::VertexType::StaticVertex * vertices = nullptr;
             //   Video::Geometry::VertexType
                uint32_t * indices = nullptr;
        
                ~MeshData() {
                    if (vertices != nullptr) {
                        delete[] vertices;
                    }
                    if (indices != nullptr) {
                        delete[] indices;
                    }
                    if (vertices != nullptr) {
                        delete[] vertices;
                    }
                }
            };
        
            /// The current version of the exporter.
            const uint16_t CURRENT_VERSION = 1;
        
            /// The lowest version supportered for importing.
            const uint16_t SUPPORTED_FROM = 1;
        
            /// Default constructor.
            AssetFileHandler();
        
            /// Open a .wkbf file.
            /**
             * @param filepath Path of the file.
             * @param mode WRITE or READ from the file.
             */
            AssetFileHandler(const char* filepath, Mode mode = READ);
        
            /// Destructor.
            ~AssetFileHandler();
        
            /// Open a .wkbf file.
            /**
             * @param filepath Path of the file.
             * @param mode WRITE or READ from the file.
             */
            bool Open(const char* filepath, Mode mode = READ);
        	
        	/// Close the opened file.
            void Close();
        
            /// Clear current data.
            void Clear();
        
            /// Load a mesh into memory.
            /**
             * @param Id of the mesh. Use GetNumMeshes() to get the number of meshes.
             */
            void LoadMeshData(int meshID);
            
            /// Get static vertex data of a mesh.
            /**
             * First load a mesh into memory by using LoadMeshData().
             * @return Static mesh data.
             */
            MeshData * GetStaticMeshData();
        
            /// Save the meshdata.
        	/**
        	 * @param meshData Static mesh data.
        	 */
<<<<<<< HEAD
        	void SaveStaticMesh(AssetFileHandler::MeshData * meshData);
=======
            void SaveStaticMesh(AssetFileHandler::StaticMeshData * meshData);
>>>>>>> 08a0b9075b8d34629f136fad77d93c1f90432e44
        
        private:
            void ReadGlobalHeader();
            void WriteGlobalHeader();
            void ClearMesh();
<<<<<<< HEAD
        
        	Mode mode;
        
        	uint16_t uniqueID;
        	uint16_t numStaticMeshes;
        
            MeshData * staticMesh = nullptr;
        
        	std::ifstream rFile;
        	std::ofstream wFile;
        	int fileVersion;
        
        	std::streampos globalHeaderStart;
        	std::vector<std::streampos> staticMeshHeaderStart;
=======
            
            Mode mode;
            
            uint16_t uniqueID;
            uint16_t numStaticMeshes;
            
            StaticMeshData * staticMesh = nullptr;
            
            std::ifstream rFile;
            std::ofstream wFile;
            int fileVersion;
            
            std::streampos globalHeaderStart;
            std::vector<std::streampos> staticMeshHeaderStart;
>>>>>>> 08a0b9075b8d34629f136fad77d93c1f90432e44
    };
}
