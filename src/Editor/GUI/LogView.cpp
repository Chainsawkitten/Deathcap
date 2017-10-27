#include "LogView.hpp"
#include <imgui.h>
#include <array>
#include <Engine/MainWindow.hpp>
#include "../ImGui/Splitter.hpp"
#include <Utility/Log.hpp>
#include <iostream>

using namespace GUI;


LogView::LogView() {
    Log().SetupStream(Log::DEFAULT, &defaultStringstream);
    Log().SetupStream(Log::INFO, &infoStringstream);
    Log().SetupStream(Log::WARNING, &warningStringstream);
    Log().SetupStream(Log::ERR, &errorStringstream);
}

void LogView::Show() {
    ImVec2 size(MainWindow::GetInstance()->GetSize().x, MainWindow::GetInstance()->GetSize().y);

    // Splitter.
    ImGui::VerticalSplitter(ImVec2(sceneWidth, 0), size.x - sceneWidth - editorWidth, splitterSize, logHeight, logResize, 20, size.y - 20);
    if (logResize)
        logHeight = size.y - logHeight;

    ImGui::SetNextWindowPos(ImVec2(sceneWidth, 20));
    ImGui::SetNextWindowSize(ImVec2(size.x - sceneWidth - editorWidth, logHeight));

    //Create log output string.
    std::string output;
    if (!defaultStringstream.str().empty())
        output += "[Default] " + defaultStringstream.str();

    if (!infoStringstream.str().empty())
        output += "[Info] " + infoStringstream.str();

    if (!warningStringstream.str().empty())
        output += "[Warning] " + warningStringstream.str();

    if (!errorStringstream.str().empty())
        output += "[Error] " + errorStringstream.str();

    textBuffer.appendv(output.c_str(), nullptr);
    ImGui::Begin("Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextUnformatted(textBuffer.begin());
    ImGui::EndChild();
    ImGui::End();

    // Clear streams.
    defaultStringstream.str(std::string());
    infoStringstream.str(std::string());
    warningStringstream.str(std::string());
    errorStringstream.str(std::string());
}

bool LogView::IsVisible() {
    return visible;
}