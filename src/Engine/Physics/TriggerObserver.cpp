#include <btBulletDynamicsCommon.h>
#include <cassert>
#include "TriggerObserver.hpp"

namespace Physics {
    TriggerObserver::TriggerObserver(btRigidBody& body)
    : btCollisionWorld::ContactResultCallback(), rigidBody(body) {

    }

    btCollisionObject* TriggerObserver::GetBulletCollisionObject() {
        return &rigidBody;
    }

    TriggerObserver::IntersectionPhase TriggerObserver::GetPhase() const {
        return phase;
    }

    void TriggerObserver::PostIntersectionTest() {
        // Intersection happened; update phase accordingly.
        if (didCallback) {
            switch (phase) {
                case IntersectionPhase::Enter: {
                    phase = IntersectionPhase::Retained;
                    break;
                }
                case IntersectionPhase::Leave: {
                    phase = IntersectionPhase::Enter;
                    break;
                }
                case IntersectionPhase::None: {
                    phase = IntersectionPhase::Enter;
                    break;
                }
                default: {
                    assert(phase == IntersectionPhase::Retained);
                }
            }
        }
        // No intersection happened; update phase accordingly.
        else {
            switch (phase) {
                case IntersectionPhase::Enter: {
                    phase = IntersectionPhase::Leave;
                    break;
                }
                case IntersectionPhase::Retained: {
                    phase = IntersectionPhase::Leave;
                    break;
                }
                case IntersectionPhase::Leave: {
                    phase = IntersectionPhase::None;
                    break;
                }
                default: {
                    assert(phase == IntersectionPhase::None);
                }
            }
        }

        // Prepare for next invocation.
        didCallback = false;
    }

    // Called with each contact for our own processing. This is where we can
    // provide custom intersection handling.
    btScalar TriggerObserver::addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0, int partId0, int index0,
        const btCollisionObjectWrapper* colObj1, int partId1, int index1) {

        assert(colObj0->getCollisionObject() == &rigidBody || colObj1->getCollisionObject() == &rigidBody);
        didCallback = true;

        return 0; // Was a planned purpose, but is not used.
    }
}
