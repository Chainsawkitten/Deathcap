#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    namespace VertexType {
        struct SkinVertex {
            /// Position.
            glm::vec3 position;
            /// %Texture coordinate.
            glm::vec2 textureCoordinate;
            /// Normal.
            glm::vec3 normal;
            /// Tangent vector.
            glm::vec3 tangent;
            /// Bone IDs.
            glm::ivec4 boneIDs;
            /// Bone weights.
            glm::vec4 weights;

            /// Generate vertex buffer
            /**
             * @param vertices Pointer to vertex array.
             * @param vertexCount Number of vertices.
             * @return Vertex buffer.
             */
            static const GLuint GenerateVertexBuffer(SkinVertex* vertices, unsigned int vertexCount);

            /// Generate vertex buffer
            /**
             * @param vertexBuffer The vertex buffer.
             * @param indexBuffer The index buffer.
             * @return Vertex array.
             */
            static const GLuint GenerateVertexArray(GLuint vertexBuffer, GLuint indexBuffer);
        };
    }
}