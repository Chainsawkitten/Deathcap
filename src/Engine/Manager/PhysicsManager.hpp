#pragma once

class World;

#include <glm/glm.hpp>
#include <vector>

namespace Component {
    class Physics;
}

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

/// Updates the physics of the world.
class PhysicsManager {
    friend class Hub;
    
    public:
        /// Update world containing entities. Moves entities and updates the physics component.
        /**
         * @param world The world to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(World& world, float deltaTime);
        
    private:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsManager(PhysicsManager const&) = delete;
        void operator=(PhysicsManager const&) = delete;
        
        glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f);

        btBroadphaseInterface* broadphase = nullptr;
        btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
        btCollisionDispatcher* dispatcher = nullptr;
        btSequentialImpulseConstraintSolver* solver = nullptr;
        btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

        std::vector<Component::Physics*> components;
};
