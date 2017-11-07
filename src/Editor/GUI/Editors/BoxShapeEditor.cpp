#include "BoxShapeEditor.hpp"

#include <Engine/Component/Shape.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/PhysicsManager.hpp>
#include <Engine/Physics/Shape.hpp>
#include <imgui.h>

#ifdef USINGMEMTRACK
#include <MemTrack.hpp>
#endif

namespace GUI {
    void BoxShapeEditor::Show(Component::Shape* comp) {
        ImGui::Indent();

        if (ImGui::InputFloat("Width", &width))
            Apply(comp);

        if (ImGui::InputFloat("Height", &height))
            Apply(comp);

        if (ImGui::InputFloat("Depth", &depth))
            Apply(comp);

        ImGui::Unindent();
    }

    void BoxShapeEditor::Apply(Component::Shape* comp) {
        Managers().physicsManager->SetShape(comp, std::shared_ptr<Physics::Shape>(new Physics::Shape(Physics::Shape::Box(width, height, depth))));
    }

    bool BoxShapeEditor::SetFromShape(const Physics::Shape& shape) {
        if (shape.GetKind() == Physics::Shape::Kind::Box) {
            auto boxData = shape.GetBoxData();
            width = boxData->width;
            height = boxData->height;
            depth = boxData->depth;
            return true;
        }

        return false;
    }
}
