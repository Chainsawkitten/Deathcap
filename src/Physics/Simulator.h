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
};

}
