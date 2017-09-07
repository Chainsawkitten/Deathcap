#include "Simulator.h"

#include <btBulletDynamicsCommon.h>

namespace Physics {

Simulator::Simulator() {
    // The broadphase is used to quickly cull bodies that will not collide with
    // each other, normally by leveraging some simpler (and rough) test such as
    // bounding boxes.
    broadphase_ = new btDbvtBroadphase;

    // With the collision configuration one can configure collision detection
    // algorithms.
    collisionConfiguration_ = new btDefaultCollisionConfiguration;
    dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);

    // The solver makes objects interact by making use of gravity, collisions,
    // game logic supplied forces, and constraints.
    solver_ = new btSequentialImpulseConstraintSolver;

    // The dynamics world encompasses objects included in the simulation.
    dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfiguration_);

    // Y axis up
    dynamicsWorld_->setGravity(btVector3(0, -9.82, 0));
}

Simulator::~Simulator() {
    delete dynamicsWorld_;
    delete solver_;
    delete dispatcher_;
    delete collisionConfiguration_;
    delete broadphase_;
}

void Simulator::Simulate(float dt) {
    dynamicsWorld_->stepSimulation(dt, 10);
}

}
