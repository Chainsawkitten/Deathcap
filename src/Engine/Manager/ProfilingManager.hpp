#pragma once

#include <string>
#include <list>
#include <map>

#include <Video/Profiling/Query.hpp>
#include "../linking.hpp"

#ifdef MEASURE_VRAM
#include <d3d11_3.h>
#include <dxgi1_4.h>
#undef CreateDirectory
#endif

/// Handles profiling.
class ProfilingManager {
    friend class Hub;
    friend class Profiling;
    friend class GPUProfiling;
    
    public:
        /// Begin profiling a frame.
        ENGINE_API void BeginFrame();

        /// End profiling a frame and collect the results.
        ENGINE_API void EndFrame();

        /// Check whether %ProfilingManager is active.
        /**
         * @return Active state.
         */
        ENGINE_API bool Active() const;

        /// Set whether %ProfilingManager is active.
        /**
         * @param active Active state.
         */
        ENGINE_API void SetActive(bool active);

        /// Get number of frames being monitored.
        /**
         * @return The number of frames being stored.
         */
        ENGINE_API unsigned int GetFrameCount() const;

        /// Get the measured CPU frame times.
        /**
         * @return The CPU frame times.
         */
        ENGINE_API const float* GetCPUFrameTimes() const;

        /// Get the measured GPU frame times.
        /**
         * @return The GPU frame times.
         */
        ENGINE_API const float* GetGPUFrameTimes() const;
        
    private:
        ProfilingManager();
        ~ProfilingManager();
        ProfilingManager(ProfilingManager const&) = delete;
        void operator=(ProfilingManager const&) = delete;
        
        enum Type {
            CPU_TIME = 0,
            GPU_TIME_ELAPSED,
            GPU_SAMPLES_PASSED,
            COUNT
        };

        struct Result {
            std::string name;
            double value = 0.0;
            std::list<Result> children;
            Result* parent;
            
            Result(const std::string& name, Result* parent);
        };
        
        Result* StartResult(const std::string& name, Type type);
        void FinishResult(Result* result, Type type);
        
        void ShowResult(Result* result);

        std::string TypeToString(Type type) const;

        bool active;
        
        Result* root[Type::COUNT];
        Result* current[Type::COUNT];

        std::map<Video::Query::Type, std::list<Video::Query*>> queryPool;
        std::map<Result*, Video::Query*> queryMap;
        
        Video::Query* frameQuery;
        double frameStart;
        static const unsigned int frames = 100;
        unsigned int frame = 0;
        float frameTimes[2][frames];

#ifdef MEASURE_VRAM
        IDXGIFactory* dxgiFactory = nullptr;
        IDXGIAdapter3* dxgiAdapter3 = nullptr;
#endif
};
