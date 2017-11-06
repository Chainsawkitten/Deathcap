#include "RayIntersection.hpp"

RayIntersection::RayIntersection() {
    
}

RayIntersection::~RayIntersection() {
    
}

bool RayIntersection::RayOBBIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const Video::AxisAlignedBoundingBox& meshData, const glm::mat4& modelMatrix, float &outputDistance) const {

    float tMin = -INFINITY;
    float tMax = INFINITY;
  
    glm::vec3 worldPos = glm::vec3(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
    glm::vec3 delta = worldPos - rayOrigin;

    glm::vec3 minVec = modelMatrix * glm::vec4(meshData.minVertex, 0.f);
    glm::vec3 maxVec = modelMatrix * glm::vec4(meshData.maxVertex, 0.f);

    float minValue[3] = { minVec.x, minVec.y, minVec.z };
    float maxValue[3] = { maxVec.x, maxVec.y, maxVec.z };

    for (int i = 0; i < 3; i++) {

        glm::vec3 currentAxis(i == 0, i == 1, i == 2);
        float e = glm::dot(currentAxis, delta);
        float f = glm::dot(rayDirection, currentAxis);

        if (fabs(f) > pow(10, -10)) {

            float t1 = (e + minValue[i]) / f;
            float t2 = (e + maxValue[i]) / f;

            if (t1 > t2) {
                float w = t1; t1 = t2; t2 = w;
            }

            if (t2 < tMax)
                tMax = t2;

            if (t1 > tMin)
                tMin = t1;

            if (tMax < tMin)
                return false;

            if (tMax < 0)
                return false;

        } else {
            if (-e + minValue[i] > 0.0f || -e + maxValue[i] < 0.0f)
                return false;
        }
    }

    if (tMin > 0)
        outputDistance = tMin;
    else
        outputDistance = tMax;
    return true;
}
