#include "SkeletonEditor.hpp"
#include <Engine/Animation/Skeleton.hpp>

using namespace GUI;

SkeletonEditor::SkeletonEditor() {
}

void SkeletonEditor::Show() {
//    if (ImGui::Begin(("Model: " + model->name + "###" + std::to_string(reinterpret_cast<uintptr_t>(model))).c_str(), &visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders)) {
//    
//    }
}

void SkeletonEditor::SetSkeleton(ResourceList::ResourceFolder * folder, Animation::Skeleton skeleton) {
}

bool SkeletonEditor::IsVisable() const {
    return false;
}

void SkeletonEditor::SetVisable(bool visable) {
}

void GUI::SkeletonEditor::FileSelected(const std::string & file) {
}
