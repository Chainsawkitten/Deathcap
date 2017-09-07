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

    // Collision shapes are only used to determine collisions and have no
    // concept of physical quantities such as mass or inertia. Note that a
    // shape may be reused for multiple bodies.
    groundShape_ = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    sphereShape_ = new btSphereShape(1);

    // Motion states informs us of movement caused by physics so that we can
    // deal with those changes as needed. The transform of the ground consists
    // of identity orientation and position 1m below ground to cancel collision
    // shape 1m above ground.
    groundMotionState_ = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    sphereMotionState_ = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

    // Static ground is represented with 0 for mass and inertia. Bullet treats
    // zero mass as infinite, resulting in immovable bodies.
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState_, groundShape_, btVector3(0, 0, 0));
    groundRigidBody_ = new btRigidBody(groundRigidBodyCI);

    // When creating a rigid body we need inertia. Bullet provides a function
    // on the shape that we can use to calculate it. We also provide the mass
    // because the sphere is dynamic.
    btScalar mass = 1;
    btVector3 sphereInertia(0, 0, 0);
    sphereShape_->calculateLocalInertia(mass, sphereInertia);
    btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState_, sphereShape_, sphereInertia);
    sphereRigidBody_ = new btRigidBody(sphereRigidBodyCI);

    // Adding a rigid body to the world allows it to be simulated.
    dynamicsWorld_->addRigidBody(groundRigidBody_);
    dynamicsWorld_->addRigidBody(sphereRigidBody_);
}

Simulator::~Simulator() {
    dynamicsWorld_->removeRigidBody(sphereRigidBody_);
    dynamicsWorld_->removeRigidBody(groundRigidBody_);

    delete sphereRigidBody_;
    delete groundRigidBody_;

    delete sphereMotionState_;
    delete groundMotionState_;

    delete sphereShape_;
    delete groundShape_;

    delete dynamicsWorld_;
    delete solver_;
    delete dispatcher_;
    delete collisionConfiguration_;
    delete broadphase_;
}

void Simulator::Simulate(float dt) {
    dynamicsWorld_->stepSimulation(dt, 10);

    btTransform trans;
    sphereRigidBody_->getMotionState()->getWorldTransform(trans);

    //printf("\rSphere height: %f", trans.getOrigin().getY());
}

}
