#include "AnimationControllerEditor.hpp"
#include <Engine/Animation/AnimationController.hpp>
#include <Utility/Log.hpp>

AnimationControllerEditor::AnimationControllerEditor() {
}

AnimationControllerEditor::~AnimationControllerEditor() {
}

void AnimationControllerEditor::ShowContextMenu() {
    ImGui::Text("Text HELLO MOTHERFUCKER!");
}

void AnimationControllerEditor::DisplayNode(Animation::AnimationController::Node * node) {
    ImGui::Text("Text HELLO MOTHERFUCKER!");
}

Animation::AnimationController::Node * AnimationControllerEditor::GetNodeArray() {
    return nullptr;
}

unsigned int AnimationControllerEditor::GetNumNodes() {
    return 0;
}
