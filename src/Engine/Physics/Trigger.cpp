#include <btBulletDynamicsCommon.h>
#include "../Component/RigidBody.hpp"
#include "Trigger.hpp"
#include "TriggerObserver.hpp"

namespace Physics {
    Trigger::Trigger(const btTransform& transform) {
        trigger = new btCollisionObject();
        trigger->setWorldTransform(transform);
    }

    btCollisionObject* Trigger::GetCollisionObject() {
        return trigger;
    }

    void Trigger::Process(btCollisionWorld& world) {
        for (auto& observer : observers) {
            world.contactPairTest(trigger, observer->GetBulletCollisionObject(), *observer);
            observer->PostIntersectionTest();

            switch (observer->GetPhase()) {
                case TriggerObserver::IntersectionPhase::Retained: {
                    callbacks[reinterpret_cast<btRigidBody*>(observer->GetBulletCollisionObject())]();
                }
            }
        }
    }

    void Trigger::OnEnter(Component::RigidBody* body, std::function<void()> observer) {
        observers.push_back(std::unique_ptr<TriggerObserver>(new TriggerObserver(*body->GetBulletRigidBody())));
        callbacks[body->GetBulletRigidBody()] = observer;
    }

    void Trigger::SetCollisionShape(btCollisionShape* shape) {
        trigger->setCollisionShape(shape);
    }
}
