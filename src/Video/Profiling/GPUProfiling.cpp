#include "GPUProfiling.hpp"

#include <assert.h>
#include <Engine/Manager/Managers.hpp>

GPUProfiling::GPUProfiling(const std::string& name, Video::Query::Type type) {
    switch (type) {
        case Video::Query::TIME_ELAPSED:
            this->type = ProfilingManager::Type::GPU_TIME_ELAPSED;
            break;
        case Video::Query::SAMPLES_PASSED:
            this->type = ProfilingManager::Type::GPU_SAMPLES_PASSED;
            break;
        default:
            assert(false);
            break;
    }
    if (Managers().profilingManager->Active()) {
        result = Managers().profilingManager->StartResult(name, this->type);
    }
}

GPUProfiling::~GPUProfiling() {
    if (Managers().profilingManager->Active())
        Managers().profilingManager->FinishResult(result, type);
}
