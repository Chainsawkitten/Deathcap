#include "PhysicsManager.hpp"

#include <Physics/Simulator.hpp>
#include "../Entity/World.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Physics.hpp"

PhysicsManager::PhysicsManager() {

}

PhysicsManager::~PhysicsManager() {

}

void PhysicsManager::Update(World &world, float deltaTime) {
    std::vector<Component::Physics*> physicsObjects = this->GetComponents<Component::Physics>();
    for (Component::Physics* physicsComp : physicsObjects) {
        if (physicsComp->IsKilled())
            continue;
        
        Entity* entity = physicsComp->entity;
        // --- Velocity ---
        // Add acceleration.
        physicsComp->velocity += (physicsComp->acceleration + this->gravity * physicsComp->gravityFactor) * deltaTime;
        
        // Add retardation.
        physicsComp->velocity -= physicsComp->velocity * physicsComp->velocityDragFactor * deltaTime;
        if (glm::length(physicsComp->velocity) > 0.01f) {
            // Cap velocity.
            if (glm::length(physicsComp->velocity) > physicsComp->maxVelocity)
                physicsComp->velocity = glm::length(physicsComp->maxVelocity) / glm::length(physicsComp->velocity) * physicsComp->velocity;
        } else {
            physicsComp->velocity = glm::vec3(0.f, 0.f, 0.f);
        }
        
        // Update position.
        entity->position += physicsComp->velocity * deltaTime;
        
        // --- Angular Velocity ---
        // Add angular acceleration.
        physicsComp->angularVelocity += physicsComp->angularAcceleration * physicsComp->momentOfInertia * deltaTime;
        
        // Add drag.
        physicsComp->angularVelocity -= physicsComp->angularVelocity * physicsComp->angularDragFactor * deltaTime;
        if (glm::length(physicsComp->angularVelocity) > 0.01f) {
            // Cap angular velocity.
            if (glm::length(physicsComp->angularAcceleration) > physicsComp->maxAngularVelocity)
                physicsComp->angularAcceleration = glm::length(physicsComp->maxAngularVelocity) / glm::length(physicsComp->angularAcceleration) * physicsComp->angularAcceleration;
        } else {
            physicsComp->angularAcceleration = glm::vec3(0.f, 0.f, 0.f);
        }
        
        // Update rotation.
        entity->rotation += physicsComp->angularVelocity * 360.f * deltaTime;
    }

    this->Simulate(deltaTime);
}
