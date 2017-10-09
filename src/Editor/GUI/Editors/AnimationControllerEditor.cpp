#include "AnimationControllerEditor.hpp"
#include <Engine/Animation/AnimationController.hpp>
#include <Utility/Log.hpp>

AnimationControllerEditor::AnimationControllerEditor() {
}

AnimationControllerEditor::~AnimationControllerEditor() {
}

void AnimationControllerEditor::Show() {
    if (ImGui::Begin("Animation controller", &visible)) {

        if (animationController->animationAction.size() == 0) {
            Animation::AnimationController::AnimationAction* action = new Animation::AnimationController::AnimationAction;
            memcpy(action->nodeName, "Entry\0", 6);
            animationController->animationAction.push_back(action);
        }

        // Draw a list of nodes on the left side
        ImGui::BeginChild("node_list", ImVec2(200, 0), true);
        ImGui::Text("Actions");
        ImGui::Separator();
        for (int node_idx = 0; node_idx < animationController->animationAction.size(); node_idx++) {
            Animation::AnimationController::AnimationAction* action = animationController->animationAction[node_idx];
            ImGui::PushID(action->index);
            if (ImGui::Selectable(action->nodeName, action->index == node_selected))
                node_selected = action->index;
            if (ImGui::IsItemHovered()) {
                node_hovered_in_list = action->index;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginGroup();

        // Create our child canvas
        ImGui::Text("Graph editor");
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Checkbox("Show grid", &show_grid);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(60, 60, 70, 200));
        ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
        ImGui::PushItemWidth(120.0f);

        ImVec2 offset = ImVec2(ImGui::GetCursorScreenPos().x - scrolling.x, ImGui::GetCursorScreenPos().y - scrolling.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->ChannelsSplit(2);

        // Grid
        ImU32 GRID_COLOR = ImColor(150, 150, 150, 200);
        ImU32 GRID_COLOR2 = ImColor(110, 110, 110, 200);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(offset.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            drawList->AddLine(ImVec2(x, win_pos.y), ImVec2(x, canvas_sz.y + win_pos.y), GRID_COLOR);
        for (float y = fmodf(offset.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            drawList->AddLine(ImVec2(win_pos.x, y), ImVec2(canvas_sz.x + win_pos.x, y), GRID_COLOR2);

        // Draw connection.
        if (isDragingConnection) {
            Animation::AnimationController::AnimationAction * action = animationController->animationAction[0];
            ImVec2 outPos = ImVec2(action->pos.x + offset.x + action->size.x, action->pos.y + offset.y + (action->size.y / 2));
            ImVec2 p2 = ImGui::GetIO().MousePos;
            ImVec2 p2Offset = ImVec2(p2.x - 50, p2.y);
            drawList->AddBezierCurve(outPos, ImVec2(outPos.x + 50, outPos.y), p2Offset, p2, ImColor(0, 0, 256), 4.0f);
        }

        for (unsigned int id = 0; id < animationController->animationAction.size(); ++id) {
            Animation::AnimationController::AnimationAction * action = animationController->animationAction[id];
            ImGui::PushID(id + animationController->animationTransitions.size());

            ImGui::PopID();
        }

        // Draw animation actions.
        for (unsigned int id = 0; id < animationController->animationAction.size(); ++id) {
            Animation::AnimationController::AnimationAction * action = animationController->animationAction[id];
            ImGui::PushID(id);
            ImVec2 node_rect_min = ImVec2(offset.x + action->pos.x, offset.y + action->pos.y);

            drawList->ChannelsSetCurrent(1); // Foreground
            bool old_any_active = ImGui::IsAnyItemActive();
            ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
            ImGui::BeginGroup(); // Lock horizontal position
            ImGui::Text("Action: %s", action->nodeName);
            ImGui::InputText("Name", action->nodeName, 128);
            ImGui::DragFloat("Playback speed", &action->playbackModifier, 0.01f);
            ImGui::EndGroup();

            // Save the size of what we have emitted and whether any of the widgets are being used
            bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
            action->size = glm::vec2(ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x, ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y);
            ImVec2 node_rect_max = ImVec2(node_rect_min.x + action->size.x, node_rect_min.y + action->size.y);

            // Display node box
            drawList->ChannelsSetCurrent(0);
            ImGui::SetCursorScreenPos(node_rect_min);
            ImGui::InvisibleButton("node", ImVec2(action->size.x, action->size.y));
            if (ImGui::IsItemHovered()) {
                node_hovered_in_scene = action->index;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }

            bool node_moving_active = ImGui::IsItemActive();

            if (node_widgets_active || node_moving_active)
                node_selected = action->index;

            if (node_moving_active && ImGui::IsMouseDragging(0))
                action->pos += glm::vec2(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);

            ImU32 node_bg_color = (node_hovered_in_list == action->index || node_hovered_in_scene == action->index || (node_hovered_in_list == -1 && node_selected == action->index)) ? ImColor(220, 220, 220) : ImColor(190, 190, 190);
            drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
            drawList->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

            //  for (unsigned int in = 0; in < action->numInputSlots; ++in)
            ImVec2 outPos = ImVec2(action->pos.x + offset.x + action->size.x, action->pos.y + offset.y + (action->size.y / 2));
            if (ImGui::GetIO().MouseClicked[0]) {
                if (ImGui::GetIO().MousePos.x < outPos.x + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.x > outPos.x - NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y < outPos.y + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y > outPos.y - NODE_SLOT_RADIUS) {
                    isDragingConnection = true;
                }
            }
            drawList->AddCircleFilled(outPos, NODE_SLOT_RADIUS, ImColor(70, 70, 256, 256), 24);

            //  for (unsigned int out = 0; out < action->numOutputSlots; ++out)
            ImVec2 inPos = ImVec2(action->pos.x + offset.x, action->pos.y + offset.y + (action->size.y / 2));
            if (ImGui::GetIO().MouseClicked[0]) {
                if (ImGui::GetIO().MousePos.x < inPos.x + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.x > inPos.x - NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y < inPos.y + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y > inPos.y - NODE_SLOT_RADIUS) {
                    isDragingConnection = true;
                }
            }
            drawList->AddCircleFilled(inPos, NODE_SLOT_RADIUS, ImColor(256, 256, 70, 256), 24);

            ImGui::PopID();
        }
        drawList->ChannelsMerge();

        if (ImGui::BeginPopupContextWindow()) {

            Animation::AnimationController::AnimationAction * action = node_selected != -1 ? animationController->animationAction[0] : nullptr;
            ImVec2 scene_pos = ImVec2(ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x, ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y);
            if (action != nullptr) {
                if (ImGui::MenuItem("Delete")) {

                }
            } else {
                if (ImGui::MenuItem("Add animation action")) {
                    Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
                    std::string name = "Action #" + std::to_string(animationController->animationAction.size());
                    memcpy(newAction->nodeName, name.c_str(), name.size() + 1);
                    newAction->index = animationController->animationAction.size();
                    animationController->animationAction.push_back(newAction);
                }

                if (ImGui::MenuItem("Add animation transition")) {
                    Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
                    std::string name = "Animation transition: " + std::to_string(animationController->animationAction.size());
                    memcpy(newAction->nodeName, name.c_str(), name.size() + 1);
                    animationController->animationAction.push_back(newAction);
                }
            }

            ImGui::EndPopup();
        }

        // Scrolling
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
            scrolling = ImVec2(scrolling.x - ImGui::GetIO().MouseDelta.x, scrolling.y - ImGui::GetIO().MouseDelta.y);

        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);
        ImGui::EndGroup();

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
