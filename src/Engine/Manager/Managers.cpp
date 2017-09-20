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

    std::string name = componentType->name();

    if (name == "class Component::Animation *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::DirectionalLight *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::Lens *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::Listener *")
        soundManager->AddComponent(component, componentType);
    else if (name == "class Component::Material *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::Mesh *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::ParticleEmitter *")
        particleManager->AddComponent(component, componentType);
    else if (name == "class Component::Physics *")
        physicsManager->AddComponent(component, componentType);
    else if (name == "class Component::PointLight *")
        renderManager->AddComponent(component, componentType);
    else if (name == "class Component::Script *")
        scriptManager->AddComponent(component, componentType);
    else if (name == "class Component::SoundSource *")
        soundManager->AddComponent(component, componentType);
    else if (name == "class Component::SpotLight *")
        renderManager->AddComponent(component, componentType);
    else
        Log() << name << " not assigned to a manager!" << "\n";

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
