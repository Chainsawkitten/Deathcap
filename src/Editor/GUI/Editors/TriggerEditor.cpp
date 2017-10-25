#include "TriggerEditor.hpp"

#include <array>
#include <Engine/Component/Trigger.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/TriggerManager.hpp>
#include <Engine/Trigger/TriggerRepeat.hpp>
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
                    auto repeat = Managers().triggerManager->GetTriggerRepeat(comp);
                    // Working under the assumption that the internal trigger
                    // is set and is indeed a repeat trigger.
                    assert(repeat);
                    std::array<char, 100> name;
                    memcpy(name.data(), repeat->GetName().c_str(), std::min(repeat->GetName().size(), name.size()));
                    name[std::min(repeat->GetName().size(), name.size() - 1)] = '\0';
                    if (ImGui::InputText("Name", name.data(), name.size())) {
                        repeat->SetName(name.data());
                    }
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
