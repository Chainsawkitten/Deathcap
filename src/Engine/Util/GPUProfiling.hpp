#pragma once

#include "../Manager/ProfilingManager.hpp"
#include <string>
#include <Video/Profiling/Query.hpp>
#include "../linking.hpp"

/// Run gpu profiling.
class GPUProfiling {
    public:
        /// Start gpu profiling.
        /**
         * @param name Name of the segment.
         * @param type Type of profiling.
         */
        ENGINE_API GPUProfiling(const std::string& name, Video::Query::Type type);
        
        /// End gpu profiling.
        ENGINE_API ~GPUProfiling();
        
    private:
        ProfilingManager::Result* result;

        ProfilingManager::Type type;

        bool active;
};

#define GPUPROFILE(name, type) GPUProfiling __gpuProfileInstance(name, type)
