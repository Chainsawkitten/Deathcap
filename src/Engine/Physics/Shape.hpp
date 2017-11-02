#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "../linking.hpp"

class btCollisionShape;
class PhysicsManager;

namespace Physics {
    class Trigger;

    /// Represents a shape for physics objects and facilitates creation of
    /// underlying types.
    class Shape {
        friend class ::PhysicsManager;
        friend class Trigger;

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

            /// Parameters used to create a box shape.
            struct Box {
                Box(float width, float height, float depth)
                    : width(width), height(height), depth(depth) {}
                float width;
                float height;
                float depth;
            };

            /// Parameters used to create a cylinder shape.
            struct Cylinder {
                Cylinder(float radius, float length)
                    : radius(radius), length(length) {}
                float radius;
                float length;
            };

            /// The various kinds of shapes that are wrapped by %Shape.
            enum class Kind {
                Sphere,
                Plane,
                Box,
                Cylinder,
            };

            /// Construct a sphere shape.
            /**
             * @param params Sphere specific parameters.
             */
            ENGINE_API Shape(const Sphere& params);

            /// Construct a plane shape.
            /**
             * @param params Plane specific parameters.
             */
            ENGINE_API Shape(const Plane& params);

            /// Construct a box shape.
            /**
             * @param params Box specific parameters.
             */
            ENGINE_API Shape(const Box& params);

            /// Construct a cylinder shape.
            /**
             * @param params Cylinder specific parameters.
             */
            ENGINE_API Shape(const Cylinder& params);

            /// Destructor
            ENGINE_API ~Shape();

            /// Get the type of wrapped shape.
            /**
             * @return The type of shape.
             */
            ENGINE_API Kind GetKind() const;

            /// Get sphere data of the shape.
            /**
             * @return Sphere data, or nullptr if the shape is not a sphere.
             */
            ENGINE_API const Sphere* GetSphereData() const;

            /// Get plane data of the shape.
            /**
             * @return Plane data, or nullptr if the shape is not a plane.
             */
            ENGINE_API const Plane* GetPlaneData() const;

            /// Get box data of the shape.
            /**
             * @return Box data, or nullptr if the shape is not a box.
             */
            ENGINE_API const Box* GetBoxData() const;

            /// Get cylinder data of the shape.
            /**
             * @return Cylinder data, or nullptr if the shape is not a cylinder.
             */
            ENGINE_API const Cylinder* GetCylinderData() const;

        private:
            /// Get the wrapped Bullet shape.
            /**
             * @return The Bullet shape.
             */
            btCollisionShape* GetShape() const;

            Shape(const Shape& other) = delete;

            std::unique_ptr<btCollisionShape> shape;
            Kind kind;
            union {
                Sphere sphere;
                Plane plane;
                Box box;
                Cylinder cylinder;
            };
    };
}
