#include "Managers.hpp"

#include "ResourceManager.hpp"
#include "RenderManager.hpp"
#include "ParticleManager.hpp"
#include "PhysicsManager.hpp"
#include "SoundManager.hpp"
#include "ScriptManager.hpp"
#include "DebugDrawingManager.hpp"
#include "ProfilingManager.hpp"

#include "../Component/SuperComponent.hpp"

Hub::Hub() {
    
}

Hub& Managers() {
    static Hub instance;
    
    return instance;
}

void Hub::StartUp() {
    resourceManager = new ResourceManager();
    renderManager = new RenderManager();
    particleManager = new ParticleManager();
    physicsManager = new PhysicsManager();
    soundManager = new SoundManager();
    scriptManager = new ScriptManager();
    debugDrawingManager = new DebugDrawingManager();
    profilingManager = new ProfilingManager();
}

void Hub::ShutDown() {
    delete profilingManager;
    delete debugDrawingManager;
    delete scriptManager;
    delete soundManager;
    delete renderManager;
    delete particleManager;
    delete physicsManager;
    delete resourceManager;
}

void Hub::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {

    switch (component->GetManager())
    {
        
    case Component::SuperComponent::MANAGER::Profiling:
        profilingManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::DebugDrawing:
        debugDrawingManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Script:
        scriptManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Sound:
        soundManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Render:
        renderManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Particle:
        particleManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Physics:
        physicsManager->AddComponent(component, componentType);
        break;
    case Component::SuperComponent::MANAGER::Resource:
        resourceManager->AddComponent(component, componentType);
        break;

    default:
        break;
    }

}

void Hub::ClearComponents() {

    resourceManager->ClearComponents();
    renderManager->ClearComponents();
    particleManager->ClearComponents();
    physicsManager->ClearComponents();
    soundManager->ClearComponents();
    scriptManager->ClearComponents();
    debugDrawingManager->ClearComponents();
    profilingManager->ClearComponents();

}

void Hub::ClearKilledComponents() {

    resourceManager->ClearKilledComponents();
    renderManager->ClearKilledComponents();
    particleManager->ClearKilledComponents();
    physicsManager->ClearKilledComponents();
    soundManager->ClearKilledComponents();
    scriptManager->ClearKilledComponents();
    debugDrawingManager->ClearKilledComponents();
    profilingManager->ClearKilledComponents();

}
