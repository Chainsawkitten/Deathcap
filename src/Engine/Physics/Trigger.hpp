#pragma once

#include <functional>
#include <map>
#include <memory>

class PhysicsManager;

namespace Component {
    class RigidBody;
};

namespace Physics {
    class TriggerObserver;

    /// Represent a trigger that checks intersections of specific rigid bodies
    /// against itself.
    class Trigger {
        friend class ::PhysicsManager;

        public:
            /// Constructor.
            /**
             * @param transform The world transform of the trigger volume.
             */
            Trigger(const btTransform& transform);

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
            std::map<btRigidBody*, std::function<void()>> callbacks;
            std::vector<std::unique_ptr<TriggerObserver>> observers;
    };
}
