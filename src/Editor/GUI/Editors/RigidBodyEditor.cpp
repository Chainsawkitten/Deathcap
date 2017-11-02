#include "RigidBodyEditor.hpp"

#include <Engine/Component/RigidBody.hpp>
#include <Engine/Component/Shape.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/PhysicsManager.hpp>
#include <imgui.h>

namespace GUI {
    void RigidBodyEditor::Show(Component::RigidBody* comp) {
        GetData(comp);

        auto shapeComp = comp->entity->GetComponent<Component::Shape>();
        if (shapeComp) {
            ImGui::Indent();
            if (ImGui::InputFloat("Mass", &mass))
                Managers().physicsManager->SetMass(comp, mass);
            ImGui::Unindent();
        }
        else {
            ImGui::Indent();
            ImGui::TextWrapped("A rigid body is only valid with a complementary shape component. Please add one to allow editing this component.");
            ImGui::Unindent();
        }
    }

    void RigidBodyEditor::GetData(Component::RigidBody* comp) {
        mass = Managers().physicsManager->GetMass(comp);
    }
}