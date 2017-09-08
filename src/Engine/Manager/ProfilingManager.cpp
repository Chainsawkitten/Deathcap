#include "ProfilingManager.hpp"

#include <imgui.h>
#include <GLFW/glfw3.h>
#ifdef WIN32
#include <windows.h>
#include <psapi.h>
#endif

ProfilingManager::ProfilingManager() {
#ifdef MEASURE_VRAM
    dxgiFactory = nullptr;
    HRESULT error = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

    if (SUCCEEDED(error)) {
        IDXGIAdapter* firstAdapter;
        dxgiFactory->EnumAdapters(0, &firstAdapter);
        firstAdapter->QueryInterface(__uuidof(IDXGIAdapter3), (void**)&dxgiAdapter3);
    }
#endif
}

ProfilingManager::~ProfilingManager() {
#ifdef MEASURE_VRAM
    dxgiAdapter3->Release();
    dxgiFactory->Release();
#endif
}

void ProfilingManager::BeginFrame() {
    // Clear previous results.
    first.children.clear();
    first.name = "";
    first.duration = 0.0;
    current = nullptr;
    frameStart = glfwGetTime();
}

void ProfilingManager::ShowResults() {
    // Calculate the time of this frame.
    frameTimes[frame++] = static_cast<float>((glfwGetTime() - frameStart) * 1000.0);
    if (frame >= frames)
        frame = 0;
    
    // Show the results.
    ImGui::Begin("Profiling", nullptr, ImGuiWindowFlags_ShowBorders);
    
    ImGui::Checkbox("Sync GPU and CPU", &syncGPU);
    
    if (ImGui::CollapsingHeader("Frametimes"))
        ShowFrametimes();
    
    if (ImGui::CollapsingHeader("Breakdown")) {
        ImGui::Columns(2);
        ShowResult(first);
        ImGui::Columns(1);
    }

    if (ImGui::CollapsingHeader("Memory")) {
#ifdef WIN32
        PROCESS_MEMORY_COUNTERS_EX memoryCounters;
        GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memoryCounters), sizeof(memoryCounters));
        ImGui::Text("RAM: %u MiB", static_cast<unsigned int>(memoryCounters.PrivateUsage / 1024 / 1024));
#endif
        
#ifdef MEASURE_VRAM
        DXGI_QUERY_VIDEO_MEMORY_INFO info;
        dxgiAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);
        unsigned int memoryUsage = info.CurrentUsage;
        ImGui::Text("VRAM: %u MiB", memoryUsage / 1024 / 1024);
#endif
    }
    
    ImGui::End();
}

ProfilingManager::Result* ProfilingManager::StartResult(const std::string& name) {
    if (current == nullptr) {
        first.name = name;
        first.parent = nullptr;
        current = &first;
    } else {
        current->children.push_back(ProfilingManager::Result(name, current));
        Result* result = &current->children.back();
        current = result;
    }
    
    return current;
}

void ProfilingManager::FinishResult(Result* result, double start) {
    // Sync GPU and CPU.
    if (syncGPU) {
        ProfilingManager::Result gpuFinish("GPU Finish", current);
        double gpuFinishStart = glfwGetTime();
        glFinish();
        gpuFinish.duration = glfwGetTime() - gpuFinishStart;
        result->children.push_back(gpuFinish);
    }
    
    result->duration = glfwGetTime() - start;
    current = result->parent;
}

void ProfilingManager::ShowFrametimes() {
    ImGui::PlotLines("Frametimes", frameTimes, frames, 0, nullptr, 0.f, FLT_MAX, ImVec2(0.f, 300.f));
}

void ProfilingManager::ShowResult(Result& result) {
    ImGui::AlignFirstTextHeightToWidgets();
    int flags = result.children.empty() ? ImGuiTreeNodeFlags_Leaf : 0;
    bool expanded = ImGui::TreeNodeEx(result.name.c_str(), flags);
    
    ImGui::NextColumn();
    if (result.parent != nullptr) {
        ImGui::ProgressBar(result.duration / result.parent->duration, ImVec2(0.0f,0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    }
    ImGui::Text((std::to_string(result.duration * 1000.0) + " ms").c_str());
    ImGui::NextColumn();
    
    if (expanded) {
        double otherTime = result.duration;
        for (Result& child : result.children) {
            ShowResult(child);
            otherTime -= child.duration;
        }
        
        if (!result.children.empty()) {
            Result other("Other", &result);
            other.duration = otherTime;
            ShowResult(other);
        }
        
        ImGui::TreePop();
    }
}

ProfilingManager::Result::Result(const std::string& name, Result* parent) : name (name) {
    this->parent = parent;
}
