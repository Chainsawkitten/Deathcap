#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include "../Component/RigidBody.hpp"
#include "Shape.hpp"
#include "Trigger.hpp"
#include "TriggerObserver.hpp"

#ifdef USINGMEMTRACK
#include <MemTrackInclude.hpp>
#endif

namespace Physics {
    Trigger::Trigger(const btTransform& transform) {
        trigger.reset(new btCollisionObject());
        trigger->setWorldTransform(transform);
    }

    void Trigger::Process(btCollisionWorld& world) {
        for (auto& observer : observers) {
            world.contactPairTest(trigger.get(), observer->GetBulletCollisionObject(), *observer);
            observer->PostIntersectionTest();
        }
    }

    void Trigger::ForObserver(btRigidBody* body, const std::function<void(TriggerObserver& observer)>& fun) {
        auto obs = std::find_if(observers.begin(), observers.end(), [body](std::unique_ptr<Physics::TriggerObserver>& ptr) {
            return body == ptr->GetBulletCollisionObject();
        });

        if (obs == observers.end()) {
            obs = observers.insert(observers.end(), std::unique_ptr<TriggerObserver>(new TriggerObserver(*body)));
        }

        fun(**obs);
    }

    void Trigger::SetCollisionShape(std::shared_ptr<Shape> shape) {
        this->shape = shape;
        trigger->setCollisionShape(shape->GetShape());
    }
}
