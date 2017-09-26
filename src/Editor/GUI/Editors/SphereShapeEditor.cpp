#include "SphereShapeEditor.hpp"

#include <Engine/Component/Physics.hpp>
#include <imgui.h>

namespace GUI {

    void SphereShapeEditor::Show(Component::Physics* comp) {
        ImGui::Text("I am a sphere shape editor.");
    }

    void SphereShapeEditor::Apply(Component::Physics* comp) {

    }
}
