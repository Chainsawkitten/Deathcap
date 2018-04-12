#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/MainWindow.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ScriptManager.hpp>
#include <Engine/Manager/ProfilingManager.hpp>
#include <Engine/Manager/RenderManager.hpp>
#include <Engine/Manager/SoundManager.hpp>
#include <Engine/Manager/VRManager.hpp>
#include <Engine/Hymn.hpp>
#include <Engine/Input/Input.hpp>
#include <Engine/Util/Input.hpp>
#include <Utility/Log.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Util/GameSettings.hpp"

int main(int argc, char* argv[]) {
    if (!glfwInit())
        return 1;

    const unsigned int maxRamLimit = 256;
    const unsigned int maxVramLimit = 512;
    
    Log().SetupStreams(&std::cout, &std::cout, &std::cout, &std::cerr);

    Log() << "Game started - " << time(nullptr) << "\n";

    bool testing = false;
    bool frameLimit = false;

    // Quick fix in order to implement a testing parameter, 
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "t") {
            testing = true;
            frameLimit = true;
            Log() << "Frame and memory testing enabled\n";
        }
        
        if (std::string(argv[i]) == "dq") {
            testing = true;
            Log() << "Testing enabled. You'll have to quit manually.\n";
        }
    }
    
    int numberOfBadFrames = 0;
    double maxFrameTime = 0.0;
    double totalFrameTime = 0.0;
    double averageFrameTime = 0.0;
    unsigned int ramUsed = 0;
    unsigned int vramUsed = 0;
    unsigned int maxRamUsed = 0;
    unsigned int maxVramUsed = 0;


    MainWindow* window = new MainWindow(1920, 1080, true, false, "Deathcap", false);
    glewInit();
    window->Init(false);
    unsigned int numberOfFrames = 1;
    Input::GetInstance().SetWindow(window->GetGLFWWindow());
    Input()->AssignButton(InputHandler::WINDOWMODE, InputHandler::KEYBOARD, GLFW_KEY_F4);
    
    Managers().StartUp();

    // Load hymn from json file.
    Hymn().Load(".");
    
    // Load game settings from ini file.
    GameSettings::GetInstance().Load();
    Managers().renderManager->SetTextureReduction(static_cast<uint16_t>(GameSettings::GetInstance().GetLong("Texture Reduction")));
    Managers().renderManager->SetShadowMapSize(GameSettings::GetInstance().GetLong("Shadow Map Size"));
    
    // Load world.
    Hymn().world.Load(Hymn().GetPath() + "/" + Hymn().startupScene + ".json");

    // Compile scripts.
    Managers().scriptManager->RegisterInput();
    Managers().scriptManager->BuildAllScripts();

    // Create audio environment.
    Managers().soundManager->CreateAudioEnvironment();
    
    // Main loop.
    double targetFPS = 60.0;
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime();
    while ((!window->ShouldClose() && !frameLimit) || (frameLimit && (numberOfFrames < 600)) ) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        if (Input()->Triggered(InputHandler::WINDOWMODE)) {
            bool fullscreen, borderless;
            window->GetWindowMode(fullscreen, borderless);
            window->SetWindowMode(!fullscreen, borderless);
        }

        window->Update();
        Hymn().Update(static_cast<float>(deltaTime));
        if (Managers().vrManager->Active())
            Hymn().Render(RenderManager::HMD);
        else
            Hymn().Render(RenderManager::MONITOR);


        if ( testing )
            glFinish();
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
        if ( testing ) {
            // Frame measurements.
            double frameTime = (glfwGetTime() - lastTimeRender);
            totalFrameTime += frameTime;
            averageFrameTime = (totalFrameTime / numberOfFrames) * 1000.0;

            if (frameTime > maxFrameTime)
                maxFrameTime = frameTime;

            if (frameTime * 1000.0 > 32.0)
                numberOfBadFrames++;
        }

        long wait = static_cast<long>((1.0 / targetFPS + lastTimeRender - glfwGetTime()) * 1000000.0);
        if ( wait > 0 )
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        // Get input.
        glfwPollEvents();

        if ( testing ) {
            ramUsed = Managers().profilingManager->MeasureRAM();
            vramUsed = Managers().profilingManager->MeasureVRAM();
            
            // Correct for driver keeping a copy of video stuff in RAM.
            ramUsed -= vramUsed;

            if (ramUsed > maxRamUsed)
                    maxRamUsed = ramUsed;
            
            if (vramUsed > maxVramUsed)
                    maxVramUsed = vramUsed;

            numberOfFrames++;
        }
    }
    
    if ( testing ) {
        std::fstream myfile("Log1080_FullGame.txt", std::ios::out);
        if (myfile) {
            myfile << "Frame rundown:\n";
            myfile << "Frames: " << numberOfFrames << "\n";
            myfile << "Bad frames: " << numberOfBadFrames << "\n";
            myfile << "Percentage of bad frames: " << (numberOfBadFrames / static_cast<double>(numberOfFrames))*100.0 << "%\n";
            myfile << "Average frame time: " << averageFrameTime << " ms\n";
            myfile << "Max frame time: " << maxFrameTime * 1000.0 << " ms\n";
            myfile << "Max ram used: " << maxRamUsed << "MiB\n";
            myfile << "Max vram used: " << maxVramUsed << "MiB\n";
            myfile.close();
        }
        Log() << "Frame rundown:\n";
        Log() << "Frames: " << numberOfFrames << "\n";
        Log() << "Bad frames: " << numberOfBadFrames << "\n";
        Log() << "Percentage of bad frames: " << (numberOfBadFrames / static_cast<double>(numberOfFrames))*100.0 << "%\n";
        Log() << "Average frame time: " << averageFrameTime << " ms\n";
        Log() << "Max frame time: " << maxFrameTime * 1000.0 << " ms\n";
        Log() << "Max ram used: " << maxRamUsed << "MiB\n";
        Log() << "Max vram used: " << maxVramUsed << "MiB\n";
    }

    // Save game settings.
    GameSettings::GetInstance().Save();

    Hymn().world.Clear();
    Managers().ShutDown();
    
    delete window;
    
    glfwTerminate();

    int returnValue = 0;

    if (maxRamUsed > maxRamLimit) {
        Log() << "Ram limit exceeded.\n";
        returnValue = returnValue | 1;
    }
    
    if (maxVramUsed > maxVramLimit) {
        Log() << "Vram limit exceeded.\n";
        returnValue = returnValue | 2;
    }

    if ((numberOfBadFrames / static_cast<double>(numberOfFrames))*100.0 > 5) {
        Log() << "Frame limit exceeded.\n";
        returnValue = returnValue | 4;
    }

    Log() << "Game ended - " << time(nullptr) << "\n";
    return returnValue;
}
