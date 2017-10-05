#include "FolderNameWindow.hpp"

#include <imgui.h>

using namespace GUI;

void FolderNameWindow::Show() {
    const char* title = "Enter folder name";
    ImGui::OpenPopup(title);

    if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_ShowBorders)) {
        ImGui::InputText("Name", name, 100);
        
        if (ImGui::Button("Create", ImVec2(120, 0))) {
            visible = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            visible = false;
        }
        ImGui::EndPopup();
    }
}

void FolderNameWindow::SetVisible(bool visible) {
    this->visible = visible;
}

bool FolderNameWindow::IsVisible() const {
    return visible;
}

void FolderNameWindow::ResetName() {
    name[0] = '\0';
}

const char* FolderNameWindow::GetName() const {
    return name;
}
