#include "TriggerEditor.hpp"

#include <Engine/Component/Trigger.hpp>
#include <imgui.h>

namespace GUI {
    void TriggerEditor::Open() {
        ImGui::SetNextWindowPosCenter();
        ImGui::OpenPopup("trigger-edit");
    }

    void TriggerEditor::Show(Component::Trigger& comp) {
        if (ImGui::BeginPopupModal("trigger-edit", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {
            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
