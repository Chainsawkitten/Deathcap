#include "Editor.hpp"

#include <Engine/Util/Input.hpp>
#include "Util/EditorSettings.hpp"
#include <Engine/Hymn.hpp>
#include <Engine/Util/FileSystem.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>

Editor::Editor() {
    // Assign controls.
    Input()->AssignButton(InputHandler::CLICK, InputHandler::MOUSE, GLFW_MOUSE_BUTTON_LEFT);
    Input()->AssignButton(InputHandler::BACK, InputHandler::KEYBOARD, GLFW_KEY_BACKSPACE);
    Input()->AssignButton(InputHandler::ERASE, InputHandler::KEYBOARD, GLFW_KEY_DELETE);
    Input()->AssignButton(InputHandler::LEFT, InputHandler::KEYBOARD, GLFW_KEY_LEFT);
    Input()->AssignButton(InputHandler::RIGHT, InputHandler::KEYBOARD, GLFW_KEY_RIGHT);
    Input()->AssignButton(InputHandler::HOME, InputHandler::KEYBOARD, GLFW_KEY_HOME);
    Input()->AssignButton(InputHandler::END, InputHandler::KEYBOARD, GLFW_KEY_END);
    Input()->AssignButton(InputHandler::PLAYTEST, InputHandler::KEYBOARD, GLFW_KEY_F5);
}

void Editor::Show() {
    bool play = false;
    
    // Main menu bar.
    if (ImGui::BeginMainMenuBar()) {
        // File menu.
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Hymn", "CTRL+N")) {
                NewHymn();
            }
            if (ImGui::MenuItem("Open Hymn", "CTRL+O")) {
                OpenHymn();
            }
            ImGui::EndMenu();
        }
        
        // Play
        if (ImGui::BeginMenu("Play")) {
            if (ImGui::MenuItem("Play", "F5")) {
                play = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // Show hymn selection window.
    if (selectHymnWindow.IsVisible())
        selectHymnWindow.Show();
    
    // Show resource list.
    if (resourceList.IsVisible())
        resourceList.Show();
    
    if (Input()->Triggered(InputHandler::PLAYTEST))
        play = true;
    
    if (play)
        Play();
}

void Editor::Save() const {
    Hymn().Save();
}

bool Editor::IsVisible() const {
    return visible;
}

void Editor::SetVisible(bool visible) {
    this->visible = visible;
}

void Editor::Play() {
    Save();
    SetVisible(false);
}

void Editor::NewHymn() {
    selectHymnWindow.Scan();
    selectHymnWindow.SetClosedCallback(std::bind(&NewHymnClosed, this, std::placeholders::_1));
    selectHymnWindow.SetTitle("New Hymn");
    selectHymnWindow.SetOpenButtonName("Create");
    selectHymnWindow.SetVisible(true);
}

void Editor::NewHymnClosed(const std::string& hymn) {
    // Create new hymn
    if (!hymn.empty()) {
        Hymn().Clear();
        Hymn().SetPath(FileSystem::DataPath("Hymn to Beauty", hymn.c_str()));
        resourceList.SetVisible(true);
    }
    
    selectHymnWindow.SetVisible(false);
}

void Editor::OpenHymn() {
    selectHymnWindow.Scan();
    selectHymnWindow.SetClosedCallback(std::bind(&OpenHymnClosed, this, std::placeholders::_1));
    selectHymnWindow.SetTitle("Open Hymn");
    selectHymnWindow.SetOpenButtonName("Open");
    selectHymnWindow.SetVisible(true);
}

void Editor::OpenHymnClosed(const std::string& hymn) {
    // Open hymn.
    if (!hymn.empty()) {
        Hymn().Load(FileSystem::DataPath("Hymn to Beauty", hymn.c_str()));
        resourceList.SetVisible(true);
    }
    
    selectHymnWindow.SetVisible(false);
}
