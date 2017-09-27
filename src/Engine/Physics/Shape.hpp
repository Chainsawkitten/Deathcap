#pragma once

#include <glm/vec3.hpp>

class btCollisionShape;

namespace Physics {

    /// Represents a shape for physics objects and facilitates creation of
    /// underlying types.
    class Shape {
        public:
            /// Parameters used to create a sphere shape.
            struct Sphere {
                Sphere(float radius) : radius(radius) {}
                float radius;
            };

            /// Parameters used to create a plane shape.
            struct Plane {
                Plane(const glm::vec3& normal, float planeCoeff)
                    : normal(normal), planeCoeff(planeCoeff) {}
                glm::vec3 normal;
                float planeCoeff;
            };

            /// Construct a sphere shape.
            /**
             * @param params Sphere specific parameters.
             */
            Shape(const Sphere& params);

            /// Construct a plane shape.
            /**
             * @param params Plane specific parameters.
             */
            Shape(const Plane& params);

            /// Get the wrapped Bullet shape.
            /**
             * @return The Bullet shape.
             */
            btCollisionShape* GetShape();

        private:
            btCollisionShape* shape;
    };

}
