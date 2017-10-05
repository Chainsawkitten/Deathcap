#pragma once

#include <btBulletCollisionCommon.h>
#include <functional>
#include <map>

class PhysicsManager;

namespace Component {
    class RigidBody;
};

namespace Physics {
    class Shape;

    /// Represent a trigger that checks intersections of specific rigid bodies
    /// against itself.
    class Trigger : public btCollisionWorld::ContactResultCallback {
        friend class ::PhysicsManager;

        public:
            /// Constructor.
            /**
             * @param transform The world transform of the trigger volume.
             */
            Trigger(const btTransform& transform);

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

            /// Attach a listener for when |body| enters the trigger volume.
            /**
             * @param body Body that is to enter the trigger volume.
             * @param observer Function to call when event is fired.
             */
            void OnEnter(Component::RigidBody* body, std::function<void()> observer);

        private:
            void SetCollisionShape(btCollisionShape* shape);

        private:
            btCollisionObject* trigger = nullptr;
            std::map<btRigidBody*, std::function<void()>> observers;
    };
}
