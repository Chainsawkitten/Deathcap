#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/MainWindow.hpp>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ScriptManager.hpp>
#include <Engine/Manager/ProfilingManager.hpp>
#include <Engine/Hymn.hpp>
#include <Engine/Input/Input.hpp>
#include <Utility/Log.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <ctime>

int main() {
    if (!glfwInit())
        return 1;
    
    Log().SetupStreams(&std::cout, &std::cout, &std::cout, &std::cerr);

    Log() << "Game started - " << time(nullptr) << "\n";
    
#ifdef TESTFRAMES
    unsigned int numberOfFrames = 1;
    int numberOfBadFrames = 0;
    double maxFrameTime = 0.0;
    double totalFrameTime = 0.0;
    double averageFrameTime = 0.0;
#endif

    //MainWindow* window = new MainWindow(1280, 720, true, false, "Hymn to Beauty", false);
    MainWindow* window = new MainWindow(1920, 1080, true, false, "Hymn to Beauty", false);
    glewInit();
    window->Init(false);

    Input::GetInstance().SetWindow(window->GetGLFWWindow());
    
    Managers().StartUp();

    Hymn().Load(".");
    Hymn().world.Load(Hymn().GetPath() + "/" + Hymn().startupScene + ".json");

    // Compile scripts.
    Managers().scriptManager->RegisterInput();
    Managers().scriptManager->BuildAllScripts();
    
    // Main loop.
    double targetFPS = 60.0;
    double lastTime = glfwGetTime();
    double lastTimeRender = glfwGetTime(); 
    while (!window->ShouldClose() && numberOfFrames < 600) {
        double deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        window->Update();
        Hymn().Update(static_cast<float>(deltaTime));
        Hymn().Render();

#ifdef TESTFRAMES
        glFinish();
#endif
        
        // Swap buffers and wait until next frame.
        window->SwapBuffers();
        
#ifdef TESTFRAMES
        // Frame measurements.
        double frameTime = (glfwGetTime() - lastTimeRender);
        totalFrameTime += frameTime;
        averageFrameTime = (totalFrameTime / numberOfFrames) * 1000.0;

        if (frameTime > maxFrameTime)
            maxFrameTime = frameTime;

        if (frameTime * 1000.0 > 32.0)
            numberOfBadFrames++;
#endif

        long wait = static_cast<long>((1.0 / targetFPS + lastTimeRender - glfwGetTime()) * 1000000.0);
        if (wait > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(wait));
        lastTimeRender = glfwGetTime();
        
        // Get input.
        glfwPollEvents();

#ifdef TESTMEMORY
        int ramUsed = Managers().profilingManager->MeasureRAM();
        int vramUsed = Managers().profilingManager->MeasureVRAM();
        if (ramUsed > 256)
            Log(Log::INFO) << "DANGER! RAM LIMIT EXCEEDED\nMIB RAM used: " << ramUsed << "\n";
        if (vramUsed > 512)
            Log(Log::INFO) << "DANGER! VRAM LIMIT EXCEEDED\nMIB VRAM used: " << vramUsed << "\n";
#endif
#ifdef TESTFRAMES
        numberOfFrames++;
#endif
    }
    
#ifdef TESTFRAMES
    std::fstream myfile("Log1080_BakedAlpha.txt", std::ios::out);
    if (myfile) {
        myfile << "Frame rundown:\n";
        myfile << "Frames: " << numberOfFrames << "\n";
        myfile << "Bad frames: " << numberOfBadFrames << "\n";
        myfile << "Percentage of bad frames: " << (numberOfBadFrames / static_cast<double>(numberOfFrames))*100.0 << "%\n";
        myfile << "Average frame time: " << averageFrameTime << " ms\n";
        myfile << "Max frame time: " << maxFrameTime * 1000.0 << " ms\n";
        myfile.close();
    }
    Log() << "Frame rundown:\n";
    Log() << "Frames: " << numberOfFrames << "\n";
    Log() << "Bad frames: " << numberOfBadFrames << "\n";
    Log() << "Percentage of bad frames: " << (numberOfBadFrames / static_cast<double>(numberOfFrames))*100.0 << "%\n";
    Log() << "Average frame time: " << averageFrameTime << " ms\n";
    Log() << "Max frame time: " << maxFrameTime * 1000.0 << " ms\n";
#endif


    Hymn().world.Clear();
    Managers().ShutDown();
    
    delete window;
    
    glfwTerminate();

    Log() << "Game ended - " << time(nullptr) << "\n";
    
    return 0;
}
