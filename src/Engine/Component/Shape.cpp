#include "Shape.hpp"

namespace Component {

    Json::Value Shape::Save() const {
        Json::Value component;
        return component;
    }

    void Shape::Load(const Json::Value& node) {

    }

    Shape::Shape(Entity* entity) : SuperComponent(entity) {

    }

}
