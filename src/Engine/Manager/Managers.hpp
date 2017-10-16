#pragma once

#include <typeinfo>
#include "../linking.hpp"

class ResourceManager;
class RenderManager;
class ParticleManager;
class PhysicsManager;
class SoundManager;
class ScriptManager;
class DebugDrawingManager;
class ProfilingManager;
class VRManager;

class World;

namespace Component {
    class SuperComponent;
}

/// Singleton class that holds all subsystems.
class ENGINE_EXPORT Hub {
    friend Hub& Managers();
    
    public:
        /// The resource manager instance.
        ResourceManager* resourceManager;
        
        /// The render manager instance.
        RenderManager* renderManager;
        
        /// The particle manager instance.
        ParticleManager* particleManager;
        
        /// The physics manager instance.
        PhysicsManager* physicsManager;
        
        /// The sound manager instance.
        SoundManager* soundManager;
        
        /// The script manager instance.
        ScriptManager* scriptManager;
        
        /// The debug drawing manager instance.
        DebugDrawingManager* debugDrawingManager;
        
        /// The profiling manager instance.
        ProfilingManager* profilingManager;

        /// The VR manager instance.
        VRManager* vrManager;
        
        /// Initialize all subsystems.
        void StartUp();
        
        /// Deinitialize all subsystems.
        void ShutDown();
        
        /// Clears the killed components of all managers.
        void ClearKilledComponents();
        
    private:
        Hub();
        Hub(const Hub&) = delete;
        void operator=(const Hub&) = delete;
        
        bool shutdown = false;
};

/// Get a hub containing all the subsystems.
/**
 * @return A hub containing all the subsystems.
 */
ENGINE_EXPORT Hub& Managers();
