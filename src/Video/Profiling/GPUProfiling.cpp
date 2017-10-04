#include "GPUProfiling.hpp"

#include <Engine/Manager/Managers.hpp>

using namespace std;

GPUProfiling::GPUProfiling(const std::string& name) {
    if (Managers().profilingManager->Active())
        result = Managers().profilingManager->StartResult(name, ProfilingManager::Type::GPU);
}

GPUProfiling::~GPUProfiling() {
    if (Managers().profilingManager->Active())
        Managers().profilingManager->FinishResult(result, ProfilingManager::Type::GPU);
}
