#pragma once

#include <glm/glm.hpp>
#include <Physics/Simulator.hpp>
#include "../Entity/ComponentContainer.hpp"

namespace Component {
    class Physics;
}

/// Updates the physics of the world.
class PhysicsManager : public Physics::Simulator {
    friend class Hub;
    
    public:
        /// Update world containing entities. Moves entities and updates the physics component.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(float deltaTime);
        
        /// Create physics component.
        /**
         * @return The created component.
         */
        Component::Physics* CreatePhysics();
        
        /// Get all physics components.
        /**
         * @return All physics components.
         */
        const std::vector<Component::Physics*>& GetPhysicses() const;
        
        /// Remove all killed components.
        void ClearKilledComponents();
        
    private:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsManager(PhysicsManager const&) = delete;
        void operator=(PhysicsManager const&) = delete;
        
        glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f);
        
        ComponentContainer<Component::Physics> physicses;
};
