#include "GPUProfiling.hpp"

#include <Engine/Manager/Managers.hpp>

using namespace std;

GPUProfiling::GPUProfiling(const std::string& name) {
    result = Managers().profilingManager->StartResult(name, ProfilingManager::Type::GPU);
}

GPUProfiling::~GPUProfiling() {
    Managers().profilingManager->FinishResult(result, ProfilingManager::Type::GPU);
}
