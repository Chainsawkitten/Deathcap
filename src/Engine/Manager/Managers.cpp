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

    ClearAllComponents();

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

void Hub::AddComponent(World* world, Component::SuperComponent* component, const std::type_info* componentType) {

    std::string name = componentType->name();

    if (name == "class Component::Animation *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::DirectionalLight *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Lens *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Listener *")
        soundManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Material *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Mesh *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::ParticleEmitter *")
        particleManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Physics *")
        physicsManager->AddComponent(world, component, componentType);
    else if (name == "class Component::PointLight *")
        renderManager->AddComponent(world, component, componentType);
    else if (name == "class Component::Script *")
        scriptManager->AddComponent(world, component, componentType);
    else if (name == "class Component::SoundSource *")
        soundManager->AddComponent(world, component, componentType);
    else if (name == "class Component::SpotLight *")
        renderManager->AddComponent(world, component, componentType);
    else
        Log() << name << " not assigned to a manager!" << "\n";

}

void Hub::ClearComponents(World* world) {

    if (!shutdown) {

        resourceManager->ClearComponents(world);
        renderManager->ClearComponents(world);
        particleManager->ClearComponents(world);
        physicsManager->ClearComponents(world);
        soundManager->ClearComponents(world);
        scriptManager->ClearComponents(world);
        debugDrawingManager->ClearComponents(world);
        profilingManager->ClearComponents(world);

    }

}

void Hub::ClearAllComponents() {

    if (!shutdown) {

        resourceManager->ClearAllComponents();
        renderManager->ClearAllComponents();
        particleManager->ClearAllComponents();
        physicsManager->ClearAllComponents();
        soundManager->ClearAllComponents();
        scriptManager->ClearAllComponents();
        debugDrawingManager->ClearAllComponents();
        profilingManager->ClearAllComponents();

    }

}

void Hub::ClearKilledComponents(World* world) {

    if (!shutdown) {

        resourceManager->ClearKilledComponents(world);
        renderManager->ClearKilledComponents(world);
        particleManager->ClearKilledComponents(world);
        physicsManager->ClearKilledComponents(world);
        soundManager->ClearKilledComponents(world);
        scriptManager->ClearKilledComponents(world);
        debugDrawingManager->ClearKilledComponents(world);
        profilingManager->ClearKilledComponents(world);

    }


}
