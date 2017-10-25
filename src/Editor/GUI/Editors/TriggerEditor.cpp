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
            if (ImGui::RadioButton("Properties", selectedTab == 0))
                selectedTab = 0;

            ImGui::SameLine();

            if (ImGui::RadioButton("Subjects", selectedTab == 1))
                selectedTab = 1;

            ImGui::SameLine();

            if (ImGui::Button("Close")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::Separator();

            switch (selectedTab) {
                case 0: {
                    ImGui::Text("I am properties");
                    break;
                }
                case 1: {
                    ImGui::Text("I am subjects");
                    break;
                }
            }

            ImGui::EndPopup();
        }
    }
}
