#include "LogView.hpp"
#include <imgui.h>
#include <array>
#include <Engine/MainWindow.hpp>
#include "../ImGui/Splitter.hpp"
#include <Utility/Log.hpp>
#include <iostream>

using namespace GUI;


LogView::LogView() {
    defaultBuffer = new std::basic_stringbuf<char>();
    infoBuffer = new std::basic_stringbuf<char>();
    warningBuffer = new std::basic_stringbuf<char>();
    errorBuffer = new std::basic_stringbuf<char>();

    defaultStream = new LogStream(defaultBuffer);
    infoStream = new LogStream(infoBuffer);
    warningStream = new LogStream(warningBuffer);
    errorStream = new LogStream(errorBuffer);
    Log().SetupStreams(defaultStream, infoStream, warningStream, errorStream, &std::cout);
}

LogView::~LogView() {
    delete defaultBuffer;
    delete infoBuffer;
    delete warningBuffer;
    delete errorBuffer;

    delete defaultStream;
    delete infoStream;
    delete warningStream;
    delete errorStream;
}

void LogView::Show() {
    ImVec2 size(MainWindow::GetInstance()->GetSize().x, MainWindow::GetInstance()->GetSize().y);

    // Splitter.
    ImGui::VerticalSplitter(ImVec2(sceneWidth, 0), size.x - sceneWidth - editorWidth, splitterSize, logHeight, logResize, 20, size.y - 20);
    if (logResize)
        logHeight = size.y - logHeight;

    ImGui::SetNextWindowPos(ImVec2(sceneWidth, 20));
    ImGui::SetNextWindowSize(ImVec2(size.x - sceneWidth - editorWidth, logHeight));

    std::string textMessage = stringBuffer->str() + "\n";

    textBuffer.appendv(textMessage.c_str(), nullptr);

    ImGui::Begin("Log", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextUnformatted(textBuffer.begin());
    ImGui::EndChild();
    ImGui::End();
}

bool LogView::IsVisible() {
    return visible;
}

LogView::LogStream::LogStream(std::stringbuf* stringBuffer) : std::ostream(stringBuffer) {

}
