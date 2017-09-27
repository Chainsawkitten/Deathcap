#include "Physics.hpp"

#include "../Physics/RigidBody.hpp"
#include "../Physics/Shape.hpp"
#include "../Util/Json.hpp"

namespace Component {

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

    void Physics::Load(const Json::Value& node) {
        delete rigidBody;
        rigidBody = nullptr;

        velocity = Json::LoadVec3(node["velocity"]);
        maxVelocity = node.get("maxVelocity", 20.f).asFloat();
        angularVelocity = Json::LoadVec3(node["angularVelocity"]);
        maxAngularVelocity = node.get("maxAngularVelocity", 2.f).asFloat();
        acceleration = Json::LoadVec3(node["acceleration"]);
        angularAcceleration = Json::LoadVec3(node["angularAcceleration"]);
        velocityDragFactor = node.get("velocityDragFactor", 1.f).asFloat();
        angularDragFactor = node.get("angularDragFactor", 1.f).asFloat();
        gravityFactor = node.get("gravityFactor", 0.f).asFloat();
        momentOfInertia = Json::LoadVec3(node["momentOfInertia"]);

        auto shape = node.get("shape", {});
        if (shape.isMember("sphere")) {
            auto sphere = shape.get("sphere", {});
            auto radius = sphere.get("radius", 1.0f).asFloat();
            auto shape = new ::Physics::Shape(::Physics::Shape::Sphere(radius));
            rigidBody = new ::Physics::RigidBody(shape, 1.0f);
        }
        else if (shape.isMember("plane")) {
            auto plane = shape.get("plane", {});
            auto normal = Json::LoadVec3(plane.get("normal", {}));
            auto planeCoeff = plane.get("planeCoeff", 0.0f).asFloat();
            auto shape = new ::Physics::Shape(::Physics::Shape::Plane(normal, planeCoeff));
            rigidBody = new ::Physics::RigidBody(shape, 1.0f);
        }

        assert(rigidBody);
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

    Physics::Physics(Entity* entity) : SuperComponent(entity) {
        // Temporary until creation via managers works properly (this is just
        // to have some default resources).
        auto shape = new ::Physics::Shape(::Physics::Shape::Sphere(2.0f));
        rigidBody = new ::Physics::RigidBody(shape, 1.0f);
    }

}
