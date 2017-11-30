#include <cinttypes>
#include <glm/glm.hpp>

namespace Video {
    namespace Geometry {
        namespace VertexType {
            struct StaticVertex;
            struct SkinVertex;
        }
    }
}
namespace Geometry {
    /// Mesh data.
    struct MeshData {
        /// Destructor.
        ~MeshData();

        uint32_t parent;
        
        uint32_t numVertices;
        
        uint32_t numIndices;
        
        glm::vec3 aabbDim;
        
        glm::vec3 aabbOrigin;
        
        glm::vec3 aabbMinpos;
        
        glm::vec3 aabbMaxpos;
        
        bool isSkinned = false;
        
        Video::Geometry::VertexType::StaticVertex * staticVertices = nullptr;
        
        Video::Geometry::VertexType::SkinVertex * skinnedVertices = nullptr;
        
        uint32_t * indices = nullptr;
        
        /// Store in cpu?
        bool CPU;
        
        /// Store in gpu?
        bool GPU;
    };
}