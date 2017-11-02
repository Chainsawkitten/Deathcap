#pragma once

#include <btBulletCollisionCommon.h>
#include "GlmConversion.hpp"
#include "Shape.hpp"

namespace Physics {
    Shape::Shape(const Shape::Sphere& params) {
        // Collision shapes are only used to determine collisions and have no
        // concept of physical quantities such as mass or inertia. Note that a
        // shape may be reused for multiple bodies.
        shape.reset(new btSphereShape(static_cast<btScalar>(params.radius)));
        kind = Kind::Sphere;
        sphere = params;
    }

    Shape::Shape(const Shape::Plane& params) {
        shape.reset(new btStaticPlaneShape(glmToBt(params.normal), static_cast<btScalar>(params.planeCoeff)));
        kind = Kind::Plane;
        plane = params;
    }

    Shape::Shape(const Shape::Box& params) {
        shape.reset(new btBoxShape(btVector3(params.width * 0.5f, params.height * 0.5f, params.depth * 0.5f)));
        kind = Kind::Box;
        box = params;
    }

    Shape::~Shape() {}

    Shape::Kind Shape::GetKind() const {
        return kind;
    }

    const Shape::Sphere* Shape::GetSphereData() const {
        return kind == Kind::Sphere ? &sphere : nullptr;
    }

    const Shape::Plane* Shape::GetPlaneData() const {
        return kind == Kind::Plane ? &plane : nullptr;
    }

    const Shape::Box* Shape::GetBoxData() const {
        return kind == Kind::Box ? &box : nullptr;
    }

    btCollisionShape* Shape::GetShape() const {
        return shape.get();
    }
}
