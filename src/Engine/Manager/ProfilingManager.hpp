#pragma once

#include <string>
#include <list>

#ifdef MEASURE_VRAM
#include <d3d11_3.h>
#include <dxgi1_4.h>
#undef CreateDirectory
#endif

/// Handles profiling.
class ProfilingManager {
    friend class Hub;
    friend class Profiling;
    
    public:
        /// Begin profiling a frame.
        void BeginFrame();
        
        /// Show the results of the profiling.
        void ShowResults();
        
    private:
        ProfilingManager();
        ~ProfilingManager();
        ProfilingManager(ProfilingManager const&) = delete;
        void operator=(ProfilingManager const&) = delete;
        
        struct Result {
            std::string name;
            double duration = 0.0;
            std::list<Result> children;
            Result* parent;
            
            Result(const std::string& name, Result* parent);
        };
        
        Result* StartResult(const std::string& name);
        void FinishResult(Result* result, double start);
        
        void ShowFrametimes() const;
        void ShowResult(Result& result);
        
        Result first = Result("", nullptr);
        Result* current = nullptr;
        
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
