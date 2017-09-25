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
#include "Utility/Log.hpp"

#include "../Component/Animation.hpp"
#include "../Component/DirectionalLight.hpp"
#include "../Component/Lens.hpp"
#include "../Component/Listener.hpp"
#include "../Component/Material.hpp"
#include "../Component/Mesh.hpp"
#include "../Component/Physics.hpp"
#include "../Component/PointLight.hpp"
#include "../Component/SoundSource.hpp"
#include "../Component/SpotLight.hpp"

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
    ClearComponents();

    delete profilingManager;
    delete debugDrawingManager;
    delete scriptManager;
    delete soundManager;
    delete renderManager;
    delete particleManager;
    delete physicsManager;
    delete resourceManager;

    shutdown = true;
}

void Hub::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    if (*componentType == typeid(Component::Animation*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::DirectionalLight*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Lens*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Listener*))
        soundManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Material*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Mesh*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::ParticleEmitter*))
        particleManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Physics*))
        physicsManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::PointLight*))
        renderManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::Script*))
        scriptManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::SoundSource*))
        soundManager->AddComponent(component, componentType);
    else if (*componentType == typeid(Component::SpotLight*))
        renderManager->AddComponent(component, componentType);
    else
        Log() << componentType->name() << " not assigned to a manager!" << "\n";
}

void Hub::ClearComponents() {
    if (!shutdown) {
        resourceManager->ClearComponents();
        renderManager->ClearComponents();
        particleManager->ClearComponents();
        physicsManager->ClearComponents();
        soundManager->ClearComponents();
        scriptManager->ClearComponents();
        debugDrawingManager->ClearComponents();
        profilingManager->ClearComponents();
    }
}

void Hub::ClearKilledComponents() {
    if (!shutdown) {
        resourceManager->ClearKilledComponents();
        renderManager->ClearKilledComponents();
        particleManager->ClearKilledComponents();
        physicsManager->ClearKilledComponents();
        soundManager->ClearKilledComponents();
        scriptManager->ClearKilledComponents();
        debugDrawingManager->ClearKilledComponents();
        profilingManager->ClearKilledComponents();
    }
}
