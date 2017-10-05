#include "Profiling.hpp"

#include <GLFW/glfw3.h>
#include "../Manager/Managers.hpp"

using namespace std;

Profiling::Profiling(const std::string& name) {
    if (Managers().profilingManager->Active()) {
        result = Managers().profilingManager->StartResult(name, ProfilingManager::Type::CPU_TIME);
        start = glfwGetTime();
    }
}

Profiling::~Profiling() {
    if (Managers().profilingManager->Active()) {
        result->duration = glfwGetTime() - start;
        Managers().profilingManager->FinishResult(result, ProfilingManager::Type::CPU_TIME);
    }
}
