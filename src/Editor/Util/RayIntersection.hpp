#pragma
#include <Engine/Geometry/AssetFileHandler.hpp>
#include <Engine/Geometry/Model.hpp>

class RayIntersection {

public:

    /// Constructor.
    RayIntersection();

    /// Destructor.
    ~RayIntersection();

    bool RayOBBIntersect(glm::vec3 rayOrigin,
        glm::vec3 rayDirection,
        Video::AxisAlignedBoundingBox meshData,
        glm::mat4 modelMatrix);

private:

};