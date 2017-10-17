#pragma once

#include <GL/glew.h>
#include <Video/Culling/AxisAlignedBoundingBox.hpp>
#include <vector>
#include "../linking.hpp"

namespace Video {
    namespace Geometry {
        /// Renderable 3D geometry.
        class Geometry3D {
            public:
                /// Type of layout of vertex points.
                enum Type {
                    STATIC = 0, ///< Default3D vertex layout (Default3D.vert).
                    SKIN ///< Skinning vertex layout (Skinning.vert).
                };
                
                /// Destructor.
                VIDEO_API virtual ~Geometry3D();
    
                /// Get the vertex array.
                /**
                 * @return The vertex array.
                 */
                VIDEO_API GLuint GetVertexArray() const;
                
                /// Get number of indices.
                /**
                 * @return Index count.
                 */
                VIDEO_API unsigned int GetIndexCount() const;
                
                /// Get the axis-aligned bounding box around the geometry.
                /**
                 * @return Local space axis-aligned bounding box around the geometry.
                 */
                VIDEO_API const Video::AxisAlignedBoundingBox& GetAxisAlignedBoundingBox() const;
                
                /// Get geometry type.
                /**
                 * @return Type.
                 */
                VIDEO_API virtual Type GetType() const = 0;
                
            protected:
                /// Generate vertex buffer.
                /**
                 * @param vertexBuffer Vertex buffer.
                 */
            //    virtual void GenerateVertexBuffer(GLuint& vertexBuffer) = 0;
                
                /// Generate vertex array.
                /**
                 * @param vertexBuffer Vertex buffer.
                 * @param indexBuffer Index buffer.
                 * @param vertexArray Vertex array.
                 */
            //    virtual void GenerateVertexArray(const GLuint vertexBuffer, const GLuint indexBuffer, GLuint& vertexArray) = 0;
                
                /// Generate index buffer.
                /**
                 * @param indexData Pointer to array of indices.
                 * @param indexCount Number of indices.
                 * @param indexBuffer Index buffer.
                 */
                VIDEO_API void GenerateIndexBuffer(unsigned int* indexData, unsigned int indexCount, GLuint& indexBuffer);
                
                /// Create local space axis-aligned bounding box around the geometry.
                /**
                 * @param positions Vector of vertex positions.
                 */
                VIDEO_API void CreateAxisAlignedBoundingBox(const std::vector<glm::vec3*>& positions);

                /// Create local space axis-aligned bounding box around the geometry.
                /**
                 * @param dim Vector of vertex positions.
                 * @param origin Vector of vertex positions.
                 * @param minValues Vector of vertex positions.
                 * @param maxValues Vector of vertex positions.
                 */
                VIDEO_API void CreateAxisAlignedBoundingBox(glm::vec3 dim, glm::vec3 origin, glm::vec3 minValues, glm::vec3 maxValues);
                
                /// Vertex buffer.
                GLuint vertexBuffer;
                
                /// Index buffer.
                GLuint indexBuffer;
                
                /// Vertex array.
                GLuint vertexArray;
                
            private:
                Video::AxisAlignedBoundingBox axisAlignedBoundingBox;
                unsigned int indexCount = 0;
        };
    }
}
