#pragma once

#include <functional>

namespace Physics {

    class RigidBody;

    /// Interface for triggers that can set up an observer for enter event.
    class ITrigger {
        public:
            /// Add listener for when a rigid body has entered a trigger volume.
            /**
             * @param body The body that is to enter the volume.
             * @param observer Event handler.
             */
            virtual void OnEnter(RigidBody& body, std::function<void(RigidBody& body)> observer) = 0;
    };

}
