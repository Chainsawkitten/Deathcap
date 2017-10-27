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

            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();

            ImGui::Separator();

            // Working under the assumption that the internal trigger
            // is set and is indeed a repeat trigger.
            auto repeat = Managers().triggerManager->GetTriggerRepeat(comp);
            assert(repeat);

            switch (selectedTab) {
                case 0: {
                    std::array<char, 100> name;
                    memcpy(name.data(), repeat->GetName().c_str(), std::min(repeat->GetName().size(), name.size()));
                    name[std::min(repeat->GetName().size(), name.size() - 1)] = '\0';
                    if (ImGui::InputText("Name", name.data(), name.size()))
                        repeat->SetName(name.data());

                    break;
                }
                case 1: {
                    // Hardcoded single event for demonstration purposes
                    ImGui::Columns(4);

                    ImGui::Text("Event");
                    ImGui::NextColumn();
                    ImGui::Text("Subject");
                    ImGui::NextColumn();
                    ImGui::Text("Target entity");
                    ImGui::NextColumn();
                    ImGui::Text("Script method");
                    ImGui::Separator();

                    ImGui::Columns(4);

                    std::array<const char*, 3> events = {
                        "OnEnter",
                        "OnRetain",
                        "OnLeave",
                    };
                    int eventType = 0;
                    if (ImGui::Combo("", &eventType, events.data(), events.size())) {
                        // Do something
                    }

                    ImGui::NextColumn();

                    ImGui::Text("[Select subject rigid body here]");

                    ImGui::NextColumn();

                    ImGui::Text("[Select target entity here]");

                    ImGui::NextColumn();

                    ImGui::Text("[Select script method here]");

                    break;
                }
            }

            ImGui::EndPopup();
        }
    }
}
