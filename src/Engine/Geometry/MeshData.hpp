/// Mesh data.
struct MeshData {
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
    bool CPU;
    bool GPU;

    /// Destructor.
    ~MeshData() {
        if (staticVertices != nullptr) {
            delete[] staticVertices;
        }
        if (skinnedVertices != nullptr) {
            delete[] skinnedVertices;
        }
        if (indices != nullptr) {
            delete[] indices;
        }
    }
};