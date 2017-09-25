#pragma once

#include <btBulletCollisionCommon.h>
#include <functional>
#include <map>
#include "ITrigger.hpp"

namespace Physics {

    class RigidBody;
    class Shape;

    class Trigger : public btCollisionWorld::ContactResultCallback, public ITrigger {
        friend class Simulator;

        public:
            Trigger(Shape* shape);

            // Called with each contact for our own processing. This is where we
            // can test that whether the correct listeners are intersecting.
            virtual btScalar addSingleResult(btManifoldPoint& cp,
                const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                const btCollisionObjectWrapper* colObj1, int partId1, int index1) override;

            btCollisionObject* GetCollisionObject();
            void Process(btCollisionWorld& world);

            virtual void OnEnter(RigidBody& body, std::function<void(RigidBody& body)> observer) override;

        private:
            btCollisionObject* trigger = nullptr;
            std::map<RigidBody*, std::function<void(RigidBody&)>> observers;
    };

}
