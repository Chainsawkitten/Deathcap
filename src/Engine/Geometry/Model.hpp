#pragma once

#include <Video/Geometry/Geometry3D.hpp>
#include <json/json.h>
#include "AssetFileHandler.hpp"
#include "../linking.hpp"

namespace Geometry {
    /// Interface of a model loaded from a file.
    class Model : public Video::Geometry::Geometry3D {
        public:
            /// Create new empty model.
            /**
             * The created model has to be loaded later using Load.
             */
            ENGINE_API Model();
            
            /// Destructor.
            ENGINE_API virtual ~Model();
            
            /// Save the model.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const;
            
            /// Load model from file.
            /**
             * @param name Name of the model.
             */
            ENGINE_API void Load(const std::string& name);
            
            /// Load model from file.
            /**
             * @param filename Filename (relative or absolute) to the model file.
             */
            ENGINE_API void Load(const char* filename);
            
            /// Get geometry type.
            /**
             * @return Type.
             */
            ENGINE_API Type GetType() const final;
            
            /// The name of the model.
            std::string name;
            
            /// The folder containing the model file.
            std::string path;
            
        private:
            void GenerateVertexBuffer(GLuint& vertexBuffer,
                Video::Geometry::VertexType::StaticVertex * vertices, unsigned int numVerticies);
            void GenerateVertexBuffer(GLuint& vertexBuffer,
                Video::Geometry::VertexType::SkinVertex * vertices, unsigned int numVerticies);
            void GenerateStaticVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray);
            void GenerateSkinVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray);

            AssetFileHandler assetFile;
            Type type;
    };
}
