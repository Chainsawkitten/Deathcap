#pragma once

#include <string>
#include <list>
#include <map>

#include <Video/Profiling/Query.hpp>

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
        void BeginFrame();
        
        /// Show the results of the profiling.
        void ShowResults();

        /// Check whether %ProfilingManager is active.
        /**
         * @return Active state.
         */
        bool Active() const;

        /// Set whether %ProfilingManager is active.
        /**
         * @param active Active state.
         */
        void SetActive(bool active);
        
    private:
        ProfilingManager();
        ~ProfilingManager();
        ProfilingManager(ProfilingManager const&) = delete;
        void operator=(ProfilingManager const&) = delete;
        
        enum Type {
            CPU = 0,
            GPU,
            COUNT
        };

        struct Result {
            std::string name;
            double duration = 0.0;
            std::list<Result> children;
            Result* parent;
            
            Result(const std::string& name, Result* parent);
        };
        
        Result* StartResult(const std::string& name, Type type);
        void FinishResult(Result* result, Type type);
        
        void ShowFrametimes();
        void ShowResult(Result* result);

        void ResolveQueries();

        bool active;
        
        Result* first[Type::COUNT];
        Result* current[Type::COUNT];

        std::list<Video::Query*> queryPool;
        std::map<Result*, Video::Query*> queryMap;
        
        double frameStart;
        static const unsigned int frames = 100;
        unsigned int frame = 0;
        float frameTimes[frames] = {};
        
        bool syncGPU = false;

#ifdef MEASURE_VRAM
        IDXGIFactory* dxgiFactory = nullptr;
        IDXGIAdapter3* dxgiAdapter3 = nullptr;
#endif
};
