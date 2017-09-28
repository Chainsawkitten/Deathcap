#include "Trigger.hpp"

#include <btBulletDynamicsCommon.h>
#include "../Component/Physics.hpp"
#include "RigidBody.hpp"
#include "Shape.hpp"

namespace Physics {

    Trigger::Trigger(Component::Physics* comp) : btCollisionWorld::ContactResultCallback() {
        trigger = new btCollisionObject();
        
        trigger->setCollisionShape(comp->GetShape().GetShape());
        trigger->setWorldTransform(comp->GetRigidBody().GetRigidBody()->getWorldTransform());
    }

    // Called with each contact for our own processing. This is where we can
    // test whether the given listeners are intersecting the trigger volume.
    btScalar Trigger::addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1, int partId1, int index1) {
        
        // The user pointer of the btRigidBody is our wrapping RigidBody instance.
        auto body = static_cast<btRigidBody*>(const_cast<btCollisionObject*>(colObj1->getCollisionObject()));
        
        // Call the corresponding callback
        observers[body]();
        return 0; // Was a planned purpose, but is not used.
    }

    btCollisionObject* Trigger::GetCollisionObject() {
        return trigger;
    }

    void Trigger::Process(btCollisionWorld& world) {
        for (auto& observerPair : observers) {
            world.contactPairTest(trigger, observerPair.first, *this);
        }
    }

    void Trigger::OnEnter(Component::Physics* body, std::function<void()> observer) {
        observers[body->GetRigidBody().GetRigidBody()] = observer;
    }

}
