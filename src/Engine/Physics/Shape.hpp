#pragma once

#include <glm/vec3.hpp>

class btCollisionShape;

namespace Physics {

    class Shape {
        public:
            struct Sphere {
                Sphere(float radius) : radius(radius) {}
                float radius;
            };

            struct Plane {
                Plane(const glm::vec3& normal, float planeCoeff)
                    : normal(normal), planeCoeff(planeCoeff) {}
                glm::vec3 normal;
                float planeCoeff;
            };

            /// The various kinds of shapes that are wrapped by %Shape.
            enum class Kind {
                Sphere,
                Plane,
            };

        public:
            Shape(const Sphere& params);
            Shape(const Plane& params);

            btCollisionShape* GetShape();

            /// Get the type of wrapped shape.
            /**
             * @return The type of shape.
             */
            Kind GetKind() const;

            /// Get sphere data of the shape.
            /**
             * @return Sphere data, or nullptr if the shape is not a sphere.
             */
            const Sphere* GetSphereData() const;

            /// Get plane data of the shape.
            /**
             * @return Plane data, or nullptr if the shape is not a plane.
             */
            const Plane* GetPlaneData() const;

        private:
            btCollisionShape* shape;
            Kind kind;
            union {
                Sphere sphere;
                Plane plane;
            };
    };

}
