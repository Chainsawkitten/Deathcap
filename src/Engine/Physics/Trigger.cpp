#include "Trigger.hpp"

#include <btBulletDynamicsCommon.h>
#include "RigidBody.hpp"
#include "Shape.hpp"

namespace Physics {

    Trigger::Trigger(Shape* shape) : btCollisionWorld::ContactResultCallback() {
        trigger = new btCollisionObject();
        trigger->setCollisionShape(shape->GetShape());
    }

    btScalar Trigger::addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1, int partId1, int index1) {
        assert(colObj0->getCollisionObject() != trigger);
        // The user pointer of the btRigidBody is our wrapping RigidBody instance.
        auto body = static_cast<RigidBody*>(colObj0->getCollisionObject()->getUserPointer());
        // Call the corresponding callback
        observers[body](*body);
        return 0; // Was a planned purpose, but is not used.
    }

    btCollisionObject* Trigger::GetCollisionObject() {
        return trigger;
    }

    void Trigger::Process(btCollisionWorld& world) {
        for (auto& observerPair : observers) {
            world.contactPairTest(trigger, observerPair.first->GetRigidBody(), *this);
        }
    }

    void Trigger::OnEnter(RigidBody& body, std::function<void(RigidBody& body)> observer) {
        observers[&body] = observer;
    }

}
