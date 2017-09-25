#pragma once

#include <glm/glm.hpp>
#include <Physics/Simulator.hpp>

#include "SuperManager.hpp"

/// Updates the physics of the world.
class PhysicsManager : public Physics::Simulator, public SuperManager {
    friend class Hub;
    
    public:
        /// Update world containing entities. Moves entities and updates the physics component.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(float deltaTime);
        
    private:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsManager(PhysicsManager const&) = delete;
        void operator=(PhysicsManager const&) = delete;
        
        glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f);
};
