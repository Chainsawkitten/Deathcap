#pragma once

#include <glm/glm.hpp>
#include "../linking.hpp"

namespace Video {
    class AxisAlignedBoundingBox;
    
    /// A viewing frustum.
    /**
     * Used for frustum culling.
     */
    class Frustum {
        public:
            /// Create new frustum.
            /**
             * @param matrix View-projection matrix to create frustum planes from.
             */
            VIDEO_API explicit Frustum(const glm::mat4& matrix);
            
            /// Check collision between frustum and an axis-aligned bounding box.
            /**
             * @param aabb The axis-aligned bounding box to check collision against.
             * @return Whether there was a collision
             */
            VIDEO_API bool Collide(const AxisAlignedBoundingBox& aabb) const;
            
        private:
            glm::vec4 planes[6];
    
            // The distance returned is the signed distance in normal vector units.
            // To ensure the "true" distance the plane equation has to be normalized.
            static float DistanceToPoint(const glm::vec4& plane, const glm::vec3& point);
    };
}
