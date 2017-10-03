#include "Physics.hpp"

#include <btBulletDynamicsCommon.h>
#include "../Physics/RigidBody.hpp"
#include "../Physics/Shape.hpp"
#include "../Util/Json.hpp"

namespace Component {
    Physics::Physics() {
        // Temporary until creation via managers works properly (this is just
        // to have some default resources).
        shape = new ::Physics::Shape(::Physics::Shape::Sphere(2.0f));
        rigidBody = new ::Physics::RigidBody(1.0f);
        rigidBody->GetRigidBody()->setCollisionShape(shape->GetShape());
    }
    
    Json::Value Physics::Save() const {
        Json::Value component;
        component["velocity"] = Json::SaveVec3(velocity);
        component["maxVelocity"] = maxVelocity;
        component["angularVelocity"] = Json::SaveVec3(angularVelocity);
        component["maxAngularVelocity"] = maxAngularVelocity;
        component["acceleration"] = Json::SaveVec3(acceleration);
        component["angularAcceleration"] = Json::SaveVec3(angularAcceleration);
        component["velocityDragFactor"] = velocityDragFactor;
        component["angularDragFactor"] = angularDragFactor;
        component["gravityFactor"] = gravityFactor;
        component["momentOfInertia"] = Json::SaveVec3(momentOfInertia);

        Json::Value componentShape;
        Json::Value concreteShape;
        switch (shape->GetKind()) {
            case ::Physics::Shape::Kind::Sphere: {
                auto sphereData = shape->GetSphereData();
                concreteShape["radius"] = sphereData->radius;
                componentShape["sphere"] = concreteShape;
                break;
            }
            case ::Physics::Shape::Kind::Plane: {
                auto planeData = shape->GetPlaneData();
                concreteShape["normal"] = Json::SaveVec3(planeData->normal);
                concreteShape["planeCoeff"] = planeData->planeCoeff;
                componentShape["plane"] = concreteShape;
                break;
            }
        }
        component["shape"] = componentShape;
        return component;
    }
    
    ::Physics::Shape& Physics::GetShape() {
        return *shape;
    }
    
    void Physics::SetShape(::Physics::Shape* shape) {
        delete this->shape;
        this->shape = shape;
        this->rigidBody->GetRigidBody()->setCollisionShape(shape->GetShape());
    }

    ::Physics::RigidBody& Physics::GetRigidBody() {
        return *rigidBody;
    }
}
