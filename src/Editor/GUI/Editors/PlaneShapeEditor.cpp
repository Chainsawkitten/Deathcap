#include "PlaneShapeEditor.hpp"

#include <Engine/Component/Physics.hpp>
#include <imgui.h>

namespace GUI {

    void PlaneShapeEditor::Show(Component::Physics* comp) {
        ImGui::Text("I am a plane shape editor.");
    }

    void PlaneShapeEditor::Apply(Component::Physics* comp) {

    }
}
