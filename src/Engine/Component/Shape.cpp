#include <Utility/Log.hpp>
#include "../Physics/Shape.hpp"
#include "../Util/Json.hpp"
#include "Shape.hpp"

namespace Component {
    Json::Value Shape::Save() const {
        Json::Value component;

        Json::Value concreteShape;
        switch (shape->GetKind()) {
            case Physics::Shape::Kind::Sphere: {
                auto sphereData = shape->GetSphereData();
                concreteShape["radius"] = sphereData->radius;
                component["sphere"] = concreteShape;
                break;
            }
            case Physics::Shape::Kind::Plane: {
                auto planeData = shape->GetPlaneData();
                concreteShape["normal"] = Json::SaveVec3(planeData->normal);
                concreteShape["planeCoeff"] = planeData->planeCoeff;
                component["plane"] = concreteShape;
                break;
            }
            case Physics::Shape::Kind::Box: {
                auto boxData = shape->GetBoxData();
                concreteShape["width"] = boxData->width;
                concreteShape["height"] = boxData->height;
                concreteShape["depth"] = boxData->depth;
                component["box"] = concreteShape;
                break;
            }
            default:
                Log(Log::ERR) << "Component::Shape::Save: Unsupported shape `" << static_cast<uint32_t>(shape->GetKind()) << "`.\n";
        }

        return component;
    }

    std::shared_ptr<Physics::Shape> Shape::GetShape() const {
        return shape;
    }

    void Shape::SetShape(std::shared_ptr<Physics::Shape> shape) {
        this->shape = shape;
    }
}
