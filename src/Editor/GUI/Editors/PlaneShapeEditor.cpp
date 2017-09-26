#include "PlaneShapeEditor.hpp"

#include <Engine/Component/Physics.hpp>
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

    }
}
