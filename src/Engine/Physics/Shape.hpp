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

        public:
            Shape(const Sphere& params);
            Shape(const Plane& params);

            btCollisionShape* GetShape();

        private:
            btCollisionShape* shape;
    };

}
