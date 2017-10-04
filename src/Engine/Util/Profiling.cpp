#include "Profiling.hpp"

#include <GLFW/glfw3.h>
#include "../Manager/Managers.hpp"

using namespace std;

Profiling::Profiling(const std::string& name) {
    result = Managers().profilingManager->StartResult(name, ProfilingManager::Type::CPU);
    start = glfwGetTime();
}

Profiling::~Profiling() {
    result->duration = glfwGetTime() - start;
    Managers().profilingManager->FinishResult(result, ProfilingManager::Type::CPU);
}
