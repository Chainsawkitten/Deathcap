#pragma once

#include <Video/Geometry/Geometry2D.hpp>

namespace Video {
    class Shader;
    class ShaderProgram;
}

namespace Geometry {
    /// A renderable 2D rectangle.
    class Rectangle : public Video::Geometry::Geometry2D {
        public:
            /// Constructor.
            Rectangle();
            
            /// Destructor.
            ~Rectangle() final;
            
            /// Get all the vertices.
            /**
             * @return Array of vertices
             */
            Vertex* GetVertices() const final;
            
            /// Get the number of vertices.
            /**
             * @return The number of vertices
             */
            unsigned int GetVertexCount() const final;
            
            /// Get all the vertex indices.
            /**
             * @return Array of vertex indices
             */
            unsigned int* GetIndices() const final;
            
            /// Get the number of indicies.
            /**
             * @return The number of vertex indices.
             */
            unsigned int GetIndexCount() const final;
            
            /// Render the rectangle with a solid color.
            /**
             * @param position Position on the screen in pixels.
             * @param size Size in pixels.
             * @param color Color.
             */
            void Render(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color) const;
            
        private:
            Vertex* vertexData = nullptr;
            unsigned int vertexNr = 0;
            
            unsigned int* indexData = nullptr;
            unsigned int indexNr = 0;
            
            // Shaders
            Video::Shader* vertexShader;
            Video::Shader* fragmentShader;
            Video::ShaderProgram* shaderProgram;
            
    };
}
