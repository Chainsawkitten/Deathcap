#pragma once

class World;

#include <functional>
#include <glm/glm.hpp>
#include <vector>

namespace Component {
    class Physics;
}

namespace Physics {
    class RigidBody;
    class Shape;
    class Trigger;
}

#include "SuperManager.hpp"
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

/// Updates the physics of the world.
class PhysicsManager : public SuperManager {
    friend class Hub;

    public:
        /// Update world containing entities. Moves entities and updates the physics component.
        /**
         * @param world The world to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(World& world, float deltaTime);

        void OnTriggerEnter(Component::Physics* triggerBody, Component::Physics* object, std::function<void()> callback);

    private:
        PhysicsManager();
        ~PhysicsManager();
        PhysicsManager(PhysicsManager const&) = delete;
        void operator=(PhysicsManager const&) = delete;

        Physics::RigidBody* MakeRigidBody(Physics::Shape* shape, float mass);
        Physics::Trigger* MakeTrigger(Component::Physics* comp);

        glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f);

        btBroadphaseInterface* broadphase = nullptr;
        btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
        btCollisionDispatcher* dispatcher = nullptr;
        btSequentialImpulseConstraintSolver* solver = nullptr;
        btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

        std::vector<Physics::Trigger*> triggers;
};
