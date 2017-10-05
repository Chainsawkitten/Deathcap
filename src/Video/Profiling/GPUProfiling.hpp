#pragma once

#include <string>
#include <Engine/Manager/Managers.hpp>
#include <Engine/Manager/ProfilingManager.hpp>
#include "Query.hpp"

/// Run gpu profiling.
class GPUProfiling {
    public:
        /// Start gpu profiling.
        /**
         * @param name Name of the segment.
         * @param type Type of profiling.
         */
        GPUProfiling(const std::string& name, Video::Query::Type type);
        
        /// End gpu profiling.
        ~GPUProfiling();
        
    private:
        ProfilingManager::Result* result;

        ProfilingManager::Type type;
};

#define GPUPROFILE(name, type) GPUProfiling __gpuProfileInstance(name, type)
