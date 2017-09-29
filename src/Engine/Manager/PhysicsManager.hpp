#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../Entity/ComponentContainer.hpp"

namespace Component {
    class Physics;
}

namespace Physics {
    class RigidBody;
    class Shape;
    class Trigger;
}

namespace Json {
    class Value;
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
        /// Moves entities and updates the physics component.
        /**
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(float deltaTime);
        
        /// Create physics component.
        /**
         * @return The created component.
         */
        Component::Physics* CreatePhysics();
        
        /// Create physics component.
        /**
         * @param node Json node to load the component from.
         * @return The created component.
         */
        Component::Physics* CreatePhysics(const Json::Value& node);
        
        /// Get all physics components.
        /**
         * @return All physics components.
         */
        const std::vector<Component::Physics*>& GetPhysicsComponents() const;
        
        /// Remove all killed components.
        void ClearKilledComponents();
        
    private:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsManager(PhysicsManager const&) = delete;
        void operator=(PhysicsManager const&) = delete;

        Physics::RigidBody* MakeRigidBody(Physics::Shape* shape, float mass);
        Physics::Trigger* MakeTrigger(Physics::Shape* shape);

        glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f);
        
        ComponentContainer<Component::Physics> physicsComponents;
        
        btBroadphaseInterface* broadphase = nullptr;
        btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
        btCollisionDispatcher* dispatcher = nullptr;
        btSequentialImpulseConstraintSolver* solver = nullptr;
        btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

        std::vector<Physics::Trigger*> triggers;
};
