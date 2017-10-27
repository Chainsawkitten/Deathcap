#include "LogView.hpp"
#include <imgui.h>
#include <Engine/MainWindow.hpp>
#include "../ImGui/Splitter.hpp"
#include "Editor.hpp"

using namespace GUI;

void LogView::Show() {
    ImVec2 size(MainWindow::GetInstance()->GetSize().x, MainWindow::GetInstance()->GetSize().y);

    // Splitter.
    ImGui::VerticalSplitter(ImVec2(sceneWidth, 0), size.x - sceneWidth - editorWidth, splitterSize, logHeight, logResize, 20, size.y - 20);
    if (logResize)
        logHeight = size.y - logHeight;

    ImGui::SetNextWindowPos(ImVec2(sceneWidth, 20));
    ImGui::SetNextWindowSize(ImVec2(size.x - sceneWidth - editorWidth, logHeight));

    ImGui::Begin("Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);

    ImGui::End();
}

bool LogView::IsVisible() {
    return visible;
}
