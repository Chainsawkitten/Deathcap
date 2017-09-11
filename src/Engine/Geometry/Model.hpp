#pragma once

#include <Video/Geometry/Geometry3D.hpp>
#include <assimp/Importer.hpp>
#include <json/json.h>
#include "Geometry/AssetFileHandler.hpp"

namespace Geometry {
    /// Interface of a model loaded from a file.
    class Model : public Video::Geometry::Geometry3D {
        public:
            /// Create new empty model.
            /**
             * The created model has to be loaded later using Load.
             */
            Model();
            
            /// Destructor.
            virtual ~Model();
            
            /// Save the model.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const;
            
            /// Load model from JSON node.
            /**
             * @param node JSON node to load from.
             */
            void Load(const Json::Value& node);
            
            /// Load model from file.
            /**
             * @param filename Filename (relative or absolute) to the model file.
             */
            void Load(const char* filename);
            
            /// Get geometry type.
            /**
             * @return Type.
             */
            Type GetType();
            
            /// The name of the model.
            std::string name;
            
            /// The extension of the model.
            std::string extension;
            
        private:
            void GenerateVertexBuffer(GLuint& vertexBuffer,
                Video::Geometry::VertexType::StaticVertex * vertices, unsigned int numVerticies);
            void GenerateVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray);
            
            AssetFileHandler assetFile;
    };
}
