#pragma once

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btDefaultMotionState;
class btRigidBody;

namespace Physics {

class Simulator {
    public:
        Simulator();
        ~Simulator();

        void Simulate(float dt);

    private:
        btBroadphaseInterface* broadphase_ = nullptr;
        btDefaultCollisionConfiguration* collisionConfiguration_ = nullptr;
        btCollisionDispatcher* dispatcher_ = nullptr;
        btSequentialImpulseConstraintSolver* solver_ = nullptr;
        btDiscreteDynamicsWorld* dynamicsWorld_ = nullptr;

        btCollisionShape* groundShape_ = nullptr;
        btCollisionShape* sphereShape_ = nullptr;

        btDefaultMotionState* groundMotionState_ = nullptr;
        btDefaultMotionState* sphereMotionState_ = nullptr;

        btRigidBody* groundRigidBody_ = nullptr;
        btRigidBody* sphereRigidBody_ = nullptr;
};

}
