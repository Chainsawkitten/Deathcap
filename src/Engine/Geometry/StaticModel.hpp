#pragma once

#include "Model.hpp"
#include <Video/Geometry/VertexType/StaticVertex.hpp>

namespace Geometry {
    /// A static model loaded from a file.
    class StaticModel : public Model {
        public:
            /// Create new empty static model.
            /**
             * The created static model has to be loaded later using Load.
             */
            StaticModel();
            
            /// Create new static model from file.
            /**
             * @param filename Filename (relative or absolute) to the model file.
             */
            StaticModel(const char* filename);
            
            /// Destructor.
            ~StaticModel() final;
            
            /// Load static model from file.
            /**
             * @param filename Filename (relative or absolute) to the model file.
             */
            void Load(const char* filename);
            
            /// Get geometry type.
            /**
             * @return Type.
             */
            Type GetType() const final;
            
        protected:
            /// Generate vertex buffer.
            /**
             * @param vertexBuffer Vertex buffer.
             */
            void GenerateVertexBuffer(GLuint& vertexBuffer);
            
            /// Generate vertex array.
            /**
             * @param vertexBuffer Vertex buffer.
             * @param indexBuffer Index buffer.
             * @param vertexArray Vertex array.
             */
            void GenerateVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray);
            
        private:
            struct MeshEntry {
                unsigned int numIndices = 0;
                unsigned int baseVertex = 0;
                unsigned int baseIndex = 0;
            };
            
            void LoadMeshes(const aiScene* aScene);
            
            std::vector<Video::Geometry::VertexType::StaticVertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<glm::vec3*> verticesPos;
    };
}
