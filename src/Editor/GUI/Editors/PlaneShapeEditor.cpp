#include "PlaneShapeEditor.hpp"

#include <Engine/Component/Physics.hpp>
#include <Engine/Physics/Shape.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

namespace GUI {

    void PlaneShapeEditor::Show(Component::Physics* comp) {
        ImGui::Indent();

        if (ImGui::InputFloat3("Normal", normal)) {
            Apply(comp);
        }

        if (ImGui::InputFloat("Plane coeff.", &planeCoeff)) {
            Apply(comp);
        }

        ImGui::Unindent();
    }

    void PlaneShapeEditor::Apply(Component::Physics* comp) {
        Physics::Shape* shape = new Physics::Shape(Physics::Shape::Plane(glm::vec3(normal[0], normal[1], normal[2]), planeCoeff));
        comp->SetShape(shape);
    }

    bool PlaneShapeEditor::SetFromShape(const Physics::Shape& shape) {
        if (shape.GetKind() == Physics::Shape::Kind::Plane) {
            auto planeData = shape.GetPlaneData();
            normal[0] = planeData->normal.x;
            normal[1] = planeData->normal.y;
            normal[2] = planeData->normal.z;
            planeCoeff = planeData->planeCoeff;
            return true;
        }

        return false;
    }
}
