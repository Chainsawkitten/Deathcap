#pragma once

#include <btBulletCollisionCommon.h>
#include <functional>
#include <map>
#include "ITrigger.hpp"

namespace Physics {

    class RigidBody;
    class Shape;

    /// Represent a trigger that checks intersections of specific rigid bodies
    /// against itself.
    class Trigger : public btCollisionWorld::ContactResultCallback, public ITrigger {
        friend class Simulator;

        public:
            /// Constructor.
            /**
             * @param shape The shape of the trigger volume.
             */
            Trigger(Shape* shape);

            /// Overridden from btCollisionWorld::ContactResultCallback.
            virtual btScalar addSingleResult(btManifoldPoint& cp,
                const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                const btCollisionObjectWrapper* colObj1, int partId1, int index1) override;

            /// Get the wrapped Bullet collision object.
            /**
             * @return The Bullet collision object.
             */
            btCollisionObject* GetCollisionObject();

            /// Process given rigid bodies against the trigger volume.
            /**
             * @param world The world in which rigid bodies reside.
             */
            void Process(btCollisionWorld& world);

            /// Implementation of ITrigger::OnEnter.
            virtual void OnEnter(RigidBody& body, std::function<void(RigidBody& body)> observer) override;

        private:
            btCollisionObject* trigger = nullptr;
            std::map<RigidBody*, std::function<void(RigidBody&)>> observers;
    };

}
