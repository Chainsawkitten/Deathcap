#include "NodeEditor.hpp"

#include <Engine/Animation/AnimationController.hpp>
#include <Utility/Log.hpp>

NodeEditor::NodeEditor() {
}

NodeEditor::~NodeEditor() {
}

void NodeEditor::Show() {
    if (ImGui::Begin("Animation controller", &visible)) {
//        if (animationController->animationAction.size() == 0) {
//            Animation::AnimationController::AnimationAction* action = new Animation::AnimationController::AnimationAction;
//            memcpy(action->name, "Entry\0", 6);
//            action->numOutputSlots = 0;
//            action->numInputSlots = 0;
//            animationController->animationAction.push_back(action);
//        }

        // Draw a list of nodes on the left side
        ImGui::BeginChild("node_list", ImVec2(200, 0), true);
        ImGui::Text("Actions");
        ImGui::Separator();
        for (int nodeId = 0; nodeId < GetNumNodes(); ++nodeId) {
            Animation::AnimationController::Node* node = &GetNodeArray()[nodeId];
            ImGui::PushID(node->index);
            if (ImGui::Selectable(node->name, node->index == node_selected))
                node_selected = node->index;
            if (ImGui::IsItemHovered()) {
                node_hovered_in_list = node->index;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginGroup();

        // Grid.
        DrawGrid();

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
            Animation::AnimationController::Node * action = &GetNodeArray()[dragNodeIndex];
            ImVec2 outPos = ImVec2(action->pos.x + offset.x + action->size.x, action->pos.y + offset.y + (action->size.y / 2));
            ImVec2 p2 = ImGui::GetIO().MousePos;
            ImVec2 p2Offset = ImVec2(p2.x - 100.0f, p2.y);
            drawList->AddBezierCurve(outPos, ImVec2(outPos.x + 100.0f, outPos.y), p2Offset, p2, ImColor(40, 40, 40), 2.0f);
        }

        for (unsigned int id = 0; id < GetNumNodes(); ++id) {
            Animation::AnimationController::Node * action = &GetNodeArray()[id];

            for (unsigned int connections = 0; connections < action->numOutputSlots; ++connections) {
                ImVec2 p1 = ImVec2(action->pos.x + offset.x + action->size.x, action->pos.y + offset.y + (action->size.y / 2));
                ImVec2 p1Offset = ImVec2(p1.x + 100.0f, p1.y);
                Animation::AnimationController::Node * connectionNode = &GetNodeArray()[action->outputIndex[connections]];
                ImVec2 p2 = ImVec2(connectionNode->pos.x + offset.x, connectionNode->pos.y + offset.y + (connectionNode->size.y / 2));
                ImVec2 p2Offset = ImVec2(p2.x - 100.0f, p2.y);
                drawList->AddBezierCurve(p1, p1Offset, p2Offset, p2, ImColor(80, 80, 80), 2.0f);
            }
        }

        hoveredNodeIndex = -1;

        // Draw animation actions.
        for (unsigned int id = 0; id < GetNumNodes(); ++id) {
            Animation::AnimationController::Node * node = &GetNodeArray()[id];
            ImGui::PushID(id);
            ImVec2 node_rect_min = ImVec2(offset.x + node->pos.x, offset.y + node->pos.y);

            drawList->ChannelsSetCurrent(1);
            bool old_any_active = ImGui::IsAnyItemActive();
            ImGui::SetCursorScreenPos(ImVec2(node_rect_min.x + NODE_WINDOW_PADDING.x, node_rect_min.y + NODE_WINDOW_PADDING.y));
            ImGui::BeginGroup(); // Lock horizontal position
            ImGui::Text("Action: %s", node->name);
            ImGui::InputText("Name", node->name, 128);
//            ImGui::DragFloat("Playback speed", &node->playbackModifier, 0.01f);
            ImGui::EndGroup();

            // Save the size of what we have emitted and whether any of the widgets are being used
            bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
            node->size = glm::vec2(ImGui::GetItemRectSize().x + NODE_WINDOW_PADDING.x + NODE_WINDOW_PADDING.x, ImGui::GetItemRectSize().y + NODE_WINDOW_PADDING.y + NODE_WINDOW_PADDING.y);
            ImVec2 node_rect_max = ImVec2(node_rect_min.x + node->size.x, node_rect_min.y + node->size.y);

            // Display node box
            drawList->ChannelsSetCurrent(0);
            ImGui::SetCursorScreenPos(node_rect_min);
            ImGui::InvisibleButton("node", ImVec2(node->size.x, node->size.y));
            if (ImGui::IsItemHovered()) {
                node_hovered_in_scene = node->index;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }

            bool node_moving_active = ImGui::IsItemActive();

            if (node_widgets_active || node_moving_active)
                node_selected = node->index;

            if (node_moving_active && ImGui::IsMouseDragging(0))
                node->pos += glm::vec2(ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);

            ImU32 node_bg_color = (node_hovered_in_list == node->index || node_hovered_in_scene == node->index || (node_hovered_in_list == -1 && node_selected == node->index)) ? ImColor(220, 220, 220) : ImColor(190, 190, 190);
            drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
            drawList->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

            //  for (unsigned int in = 0; in < action->numInputSlots; ++in)
            ImVec2 outPos = ImVec2(node->pos.x + offset.x + node->size.x, node->pos.y + offset.y + (node->size.y / 2));
            ImVec2 inPos = ImVec2(node->pos.x + offset.x, node->pos.y + offset.y + (node->size.y / 2));
            ImColor inColor = ImColor(70, 70, 256, 256);
            ImColor outColor = ImColor(70, 256, 70, 256);

            if (ImGui::GetIO().MousePos.x < outPos.x + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.x > outPos.x - NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y < outPos.y + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y > outPos.y - NODE_SLOT_RADIUS) {
                hoveredNodeIndex = id;
                outColor = ImColor(100, 256, 100, 256);
            } else if (ImGui::GetIO().MousePos.x < inPos.x + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.x > inPos.x - NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y < inPos.y + NODE_SLOT_RADIUS && ImGui::GetIO().MousePos.y > inPos.y - NODE_SLOT_RADIUS) {
                hoveredNodeIndex = id;
                inColor = ImColor(100, 100, 256, 256);
            }

            drawList->AddCircleFilled(outPos, NODE_SLOT_RADIUS, outColor, 24);
            drawList->AddCircleFilled(inPos, NODE_SLOT_RADIUS, inColor, 24);

            ImGui::PopID();
        }

        drawList->ChannelsMerge();

//        if (ImGui::GetIO().MouseClicked[0]) {
//            if (hoveredNodeIndex == -1)
//                isDragingConnection = false;
//            else {
//                if (isDragingConnection) {
//                    if (animationController->animationAction[dragNodeIndex]->numOutputSlots < 8) {
//                        animationController->animationAction[dragNodeIndex]->outputIndex[animationController->animationAction[dragNodeIndex]->numOutputSlots] = hoveredNodeIndex;
//                        animationController->animationAction[dragNodeIndex]->numOutputSlots += 1;
//                    }
//                    isDragingConnection = false;
//                } else {
//                    dragNodeIndex = hoveredNodeIndex;
//                    isDragingConnection = true;
//                }
//            }
//        }

        DrawContextMenu();
//
//        if (ImGui::BeginPopupContextWindow("AnimCtrlContextPopup")) {
//            if (ImGui::MenuItem("Add animation action")) {
//                Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
//                std::string name = "Action #" + std::to_string(animationController->animationAction.size() + 1);
//                memcpy(newAction->name, name.c_str(), name.size() + 1);
//                newAction->index = animationController->animationAction.size();
//                newAction->pos = glm::vec2(ImGui::GetCursorScreenPos().x - offset.x, ImGui::GetCursorScreenPos().y - offset.y);
//                animationController->animationAction.push_back(newAction);
//            }
//
//            if (ImGui::MenuItem("Add animation transition")) {
//                Animation::AnimationController::AnimationAction* newAction = new Animation::AnimationController::AnimationAction;
//                std::string name = "Animation transition: " + std::to_string(animationController->animationAction.size());
//                memcpy(newAction->name, name.c_str(), name.size() + 1);
//                animationController->animationAction.push_back(newAction);
//            }
//
//            //   Animation::AnimationController::AnimationAction * action = node_selected != -1 ? animationController->animationAction[0] : nullptr;
//            //   ImVec2 scene_pos = ImVec2(ImGui::GetMousePosOnOpeningCurrentPopup().x - offset.x, ImGui::GetMousePosOnOpeningCurrentPopup().y - offset.y);
//            //   if (action != nullptr) {
//            //       ImGui::Separator();
//            //       if (ImGui::MenuItem("Delete")) {
//            //
//            //       }
//            //   }
//            ImGui::EndPopup();
//        }
//
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

bool NodeEditor::IsVisible() const {
    return visible;
}

void NodeEditor::SetVisible(bool visible) {
    this->visible = visible;
}

bool NodeEditor::CanConnect(Animation::AnimationController::Node * output, Animation::AnimationController::Node * input) {
    return false;
}

void NodeEditor::DrawGrid() {
    ImGui::Text("Graph editor");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Checkbox("Show grid", &show_grid);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);
}
