#include "Managers.hpp"

#include "ResourceManager.hpp"
#include "RenderManager.hpp"
#include "ParticleManager.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"
#include "ScriptManager.hpp"
#include "DebugDrawingManager.hpp"
#include "ProfilingManager.hpp"
#include "VRManager.hpp"
#include "TriggerManager.hpp"

#include "Utility/Log.hpp"

#include "../Component/Animation.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

Hub::Hub() {
    
}

Hub& Managers() {
    static Hub instance;
    
    return instance;
}

void Hub::StartUp() {
    resourceManager = new ResourceManager();
    vrManager = new VRManager();
    renderManager = new RenderManager();
    particleManager = new ParticleManager();
    physicsManager = new PhysicsManager();
    soundManager = new SoundManager();
    scriptManager = new ScriptManager();
    debugDrawingManager = new DebugDrawingManager();
    profilingManager = new ProfilingManager();
    triggerManager = new TriggerManager();
}

void Hub::ShutDown() {
    delete triggerManager;
    delete profilingManager;
    delete debugDrawingManager;
    delete scriptManager;
    delete soundManager;
    delete renderManager;
    delete vrManager;
    delete particleManager;
    delete physicsManager;
    delete resourceManager;
    
    shutdown = true;
}

void Hub::ClearKilledComponents() {
    if (!shutdown) {
        renderManager->ClearKilledComponents();
        particleManager->ClearKilledComponents();
        physicsManager->ClearKilledComponents();
        soundManager->ClearKilledComponents();
        scriptManager->ClearKilledComponents();
        vrManager->ClearKilledComponents();
    }
}
