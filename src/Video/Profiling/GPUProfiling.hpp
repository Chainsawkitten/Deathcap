#pragma once

#include <string>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ProfilingManager.hpp>

/// Run gpu profiling.
class GPUProfiling {
    public:
        /// Start gpu profiling.
        /**
         * @param name Name of the segment.
         */
        GPUProfiling(const std::string& name);
        
        /// End gpu profiling.
        ~GPUProfiling();
        
    private:
        ProfilingManager::Result* result;
};

#define GPUPROFILE(name) GPUProfiling __gpuProfileInstance(name)
