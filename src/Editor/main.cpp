#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/MainWindow.hpp>
#include "Editor.hpp"
#include "Util/EditorSettings.hpp"
#include <Engine/Util/FileSystem.hpp>
#include <Engine/Util/Log.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Hymn.hpp>
#include <thread>

int main() {
    // Enable logging if requested.
    if (EditorSettings::GetInstance().GetBool("Logging"))
        freopen(FileSystem::DataPath("Hymn to Beauty", "log.txt").c_str(), "a", stderr);
    
    Log() << "Editor started - " << time(nullptr) << "\n";
    
    if (!glfwInit())
        return 1;
    
    MainWindow* window = new MainWindow(EditorSettings::GetInstance().GetLong("Width"), EditorSettings::GetInstance().GetLong("Height"), false, false, "Hymn to Beauty", EditorSettings::GetInstance().GetBool("Debug Context"));
    glewInit();
    window->Init(false);
    
    Managers().StartUp();
    
    Editor* editor = new Editor();
    
    // Main loop.
    double targetFPS = 60.0;
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose()) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        window->Update();
        
        if (editor->IsVisible()) {
            editor->Update();
            editor->Render();
        } else {
            Hymn().Update(deltaTime);
            Hymn().Render();
            
            if (Input()->Triggered(InputHandler::PLAYTEST)) {
                // Reload hymn.
                std::string path = Hymn().GetPath();
                Hymn().Load(path);
                
                // Turn editor back on.
                editor->SetVisible(true);
            }
        }
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / targetFPS + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        glfwPollEvents();
    }
    
    if (editor->IsVisible())
        editor->Save();
    
    delete editor;
    
    Managers().ShutDown();
    
    delete window;
    
    glfwTerminate();
    
    EditorSettings::GetInstance().Save();
    
    Log() << "Editor ended - " << time(nullptr) << "\n";
    
    return 0;
}