#include "SphereShapeEditor.hpp"

#include <Engine/Component/Physics.hpp>
#include <Engine/Physics/Shape.hpp>
#include <imgui.h>

namespace GUI {

    void SphereShapeEditor::Show(Component::Physics* comp) {
        ImGui::Indent();

        if (ImGui::DragFloat("Radius", &radius, 1.0f, 0.0f)) {
            Apply(comp);
        }

        ImGui::Unindent();
    }

    void SphereShapeEditor::Apply(Component::Physics* comp) {
        Physics::Shape* shape = new Physics::Shape(Physics::Shape::Sphere(radius));
        //comp->SetShape(shape);
    }

    bool SphereShapeEditor::SetFromShape(const Physics::Shape& shape) {
        if (shape.GetKind() == Physics::Shape::Kind::Sphere) {
            auto sphereData = shape.GetSphereData();
            radius = sphereData->radius;
            return true;
        }

        return false;
    }
}
