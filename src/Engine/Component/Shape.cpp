#include "../Physics/Shape.hpp"
#include "Shape.hpp"

namespace Component {
    Json::Value Shape::Save() const {
        Json::Value component;
        return component;
    }

    const ::Physics::Shape& Shape::GetShape() const {
        return *shape;
    }

    void Shape::SetShape(::Physics::Shape* shape) {
        this->shape = shape;
    }
}
