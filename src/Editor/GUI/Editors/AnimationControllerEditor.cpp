#include "AnimationControllerEditor.hpp"
#include <Engine/Animation/AnimationController.hpp>
#include <imgui.h>

AnimationControllerEditor::AnimationControllerEditor() {
}

AnimationControllerEditor::~AnimationControllerEditor() {
}

void AnimationControllerEditor::Show() {
    if (ImGui::Begin("Animation controller", &visible)) {
        // Get the draw list.
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->ChannelsSplit(2);

        // Draw animation actions.
        for (unsigned int id = 0; id < animationController->animationAction.size(); ++id) {
            Animation::AnimationController::AnimationAction * action = animationController->animationAction[id];
            ImGui::PushID(id);

            drawList->ChannelsSetCurrent(1); // Foreground
            ImGui::BeginGroup(); // Lock horizontal position
            ImGui::Text("%s", action->nodeName);
            //   ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
            ImGui::EndGroup();

            // Display node box
            drawList->ChannelsSetCurrent(0); // Background
            ImGui::SetCursorScreenPos(ImVec2(100, 200));
            ImGui::InvisibleButton("node", ImVec2(100, 200));
            //if (ImGui::IsItemHovered()) {
            //    node_hovered_in_scene = node->ID;
            //    open_context_menu |= ImGui::IsMouseClicked(1);
            //}

            ImU32 node_bg_color = ImColor(75, 75, 75);
            drawList->AddRectFilled(ImVec2(100, 200), ImVec2(100, 200), node_bg_color, 4.0f);
            drawList->AddRect(ImVec2(100, 200), ImVec2(100, 200), ImColor(100, 100, 100), 4.0f);
            drawList->AddCircleFilled(ImVec2(10.0f, 20.0f), 1, ImColor(150, 150, 150, 150));
            drawList->AddCircleFilled(ImVec2(10.0f, 20.0f), 1, ImColor(150, 150, 150, 150));

            ImGui::PopID();
        }

        for (unsigned int id = 0; id < animationController->animationAction.size(); ++id) {
            Animation::AnimationController::AnimationAction * action = animationController->animationAction[id];
            ImGui::PushID(id + animationController->animationTransitions.size());

            ImGui::PopID();
        }
        
        drawList->ChannelsMerge();

        // Draw context menu
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Add animation action")) {
                Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
                std::string name = "Animation Action: " + std::to_string(animationController->animationAction.size());
                memcpy(newAction->nodeName, name.c_str(), name.size() + 1);
                animationController->animationAction.push_back(newAction);
            }

            if (ImGui::MenuItem("Add animation transition")) {
                Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
                std::string name = "Animation Action: " + std::to_string(animationController->animationAction.size());
                memcpy(newAction->nodeName, name.c_str(), name.size() + 1);
                animationController->animationAction.push_back(newAction);
            }

            ImGui::EndPopup();
        }

    }
    ImGui::End();
}

void AnimationControllerEditor::SetAnimationController(Animation::AnimationController * animationController) {
    this->animationController = animationController;
}

bool AnimationControllerEditor::IsVisible() const {
    return visible;
}

void AnimationControllerEditor::SetVisible(bool visible) {
    this->visible = visible;
}
