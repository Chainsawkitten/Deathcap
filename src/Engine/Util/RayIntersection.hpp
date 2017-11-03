#pragma once
#include <Engine/Geometry/AssetFileHandler.hpp>
#include <Engine/Geometry/Model.hpp>

/// The class handling intersection checks.
class RayIntersection {
    public:

        /// Constructor.
        RayIntersection();

        /// Destructor.
        ~RayIntersection();

        /// Checks whether a ray intersects with an OBB.
        /**
         * @param rayOrigin The position the ray originates from.
         * @param rayDirection The direction the ray is shooting towards.
         * @param meshData The bounding box belonging to the mesh that is tested against.
         * @param modelMatrix The model matrix belonging to the mesh that is tested against.
         * @param outputDistance The distance to the entity that is tested to be used as a return value check.
         * @return Whether or not the ray intersects with the entity that is tested against.
         */
        bool RayOBBIntersect(glm::vec3 rayOrigin,
            glm::vec3 rayDirection,
            Video::AxisAlignedBoundingBox meshData,
            glm::mat4 modelMatrix,
            float &outputDistance) const;

        /// Checks whether a ray intersects with a triangle.
        /**
        * @param rayOrigin The position the ray originates from.
        * @param rayDirection The direction the ray is shooting towards.
        * @param p0 The first vertex of the triangle thats being tested for intersection.
        * @param p1 The second vertex of the triangle thats being tested for intersection.
        * @param p2 The third vertex of the triangle thats being tested for intersection.
        *@param distance the distance from the origin to the intersection point.
        * @return Whether or not the ray intersects with the triangle that is tested against.
        */
        bool TriangleIntersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float &distance);
};
