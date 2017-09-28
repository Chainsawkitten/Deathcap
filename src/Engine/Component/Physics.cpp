#include "Physics.hpp"

#include "../Physics/RigidBody.hpp"
#include "../Physics/Shape.hpp"
#include "../Util/Json.hpp"

namespace Component {
    Physics::Physics() {
        // Temporary until creation via managers works properly (this is just
        // to have some default resources).
        auto shape = new ::Physics::Shape(::Physics::Shape::Sphere(2.0f));
        rigidBody = new ::Physics::RigidBody(shape, 1.0f);
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
        ::Physics::Shape& shape = rigidBody->GetShape();
        switch (shape.GetKind()) {
            case ::Physics::Shape::Kind::Sphere: {
                auto sphereData = shape.GetSphereData();
                concreteShape["radius"] = sphereData->radius;
                componentShape["sphere"] = concreteShape;
                break;
            }
            case ::Physics::Shape::Kind::Plane: {
                auto planeData = shape.GetPlaneData();
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
        return rigidBody->GetShape();
    }
    
    void Physics::SetShape(::Physics::Shape* shape) {
        rigidBody->SetShape(shape);
    }

    ::Physics::RigidBody& Physics::GetRigidBody() {
        return *rigidBody;
    }
}
