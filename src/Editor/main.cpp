#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/MainWindow.hpp>
#include "Editor.hpp"
#include "Util/EditorSettings.hpp"
#include <Engine/Util/Input.hpp>
#include <Engine/Util/FileSystem.hpp>
#include <Utility/Log.hpp>
#include <Engine/Input/Input.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ScriptManager.hpp>
#include <Engine/Manager/ProfilingManager.hpp>
#include <Engine/Manager/ParticleManager.hpp>
#include <Engine/Util/Profiling.hpp>
#include <Video/Profiling/GPUProfiling.hpp>
#include <Engine/Hymn.hpp>
#include <thread>
#include "ImGui/OpenGLImplementation.hpp"
#include <imgui.h>

#ifdef VR_SUPPORT
#include <openvr.h>
#endif

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
    
    // Init VR.
#ifdef VR_SUPPORT
    vr::EVRInitError error;
    vr::IVRSystem* vrSystem = vr::VR_Init(&error, vr::VRApplication_Scene);
    Log() << "VR init: " << error << "\n";
#endif
    
    Input::GetInstance().SetWindow(window->GetGLFWWindow());
    
    Managers().StartUp();
    
    Editor* editor = new Editor();
    // Setup imgui implementation.
    ImGuiImplementation::Init(window->GetGLFWWindow());
    
    bool profiling = false;
    
    // Main loop.
    double targetFPS = 60.0;
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while (!window->ShouldClose() || !editor->ReadyToClose()) {
        float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
        lastTime = glfwGetTime();
        
        Managers().profilingManager->SetActive(profiling);

        // Begin new profiling frame.
        if (Managers().profilingManager->Active())
            Managers().profilingManager->BeginFrame();
        
        { PROFILE("Frame"); GPUPROFILE("Frame_");
            glfwPollEvents();
            
            if (Input()->Triggered(InputHandler::PROFILE))
                profiling = !profiling;
            
            // Start new frame.
            ImGuiImplementation::NewFrame();
            
            window->Update();
            
            if (editor->IsVisible()) {
                Hymn().world.ClearKilled();
                Managers().particleManager->Update(Hymn().world, deltaTime, true);
                Hymn().Render(editor->GetCamera(), EditorSettings::GetInstance().GetBool("Sound Source Icons"), EditorSettings::GetInstance().GetBool("Particle Emitter Icons"), EditorSettings::GetInstance().GetBool("Light Source Icons"), EditorSettings::GetInstance().GetBool("Camera Icons"));
                
                if (window->ShouldClose())
                    editor->Close();
    
                editor->Show(deltaTime);

            } else {
                { PROFILE("Update");
                    Hymn().Update(deltaTime);
                }
                { PROFILE("Render");
                    Hymn().Render();
                }
                
                if (Input()->Triggered(InputHandler::PLAYTEST)) {
                    // Rollback to the editor state.
                    editor->LoadEditorState();
                    
                    // Turn editor back on.
                    editor->SetVisible(true);
                }
            }
        }
        
        if (Managers().profilingManager->Active())
            Managers().profilingManager->ShowResults();
        
        ImGui::Render();
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        long wait = static_cast<long>((1.0 / targetFPS + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
    }
    
    // Save.
    EditorSettings::GetInstance().Save();
    if (editor->IsVisible())
        editor->Save();
    
    // Shut down and cleanup.
    ImGuiImplementation::Shutdown();
    delete editor;
    Hymn().world.Clear();
    
    Managers().ShutDown();
    
    delete window;
    glfwTerminate();
    
    Log() << "Editor ended - " << time(nullptr) << "\n";
    
    return 0;
}
