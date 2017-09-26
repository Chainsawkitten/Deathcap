#pragma once

#include <btBulletCollisionCommon.h>
#include "GlmConversion.hpp"
#include "Shape.hpp"

namespace Physics {

    Shape::Shape(const Shape::Sphere& params) {
        // Collision shapes are only used to determine collisions and have no
        // concept of physical quantities such as mass or inertia. Note that a
        // shape may be reused for multiple bodies.
        shape = new btSphereShape(static_cast<btScalar>(params.radius));
    }

    Shape::Shape(const Shape::Plane& params) {
        shape = new btStaticPlaneShape(glmToBt(params.normal), static_cast<btScalar>(params.planeCoeff));
    }

    btCollisionShape* Shape::GetShape() {
        return shape;
    }

}
