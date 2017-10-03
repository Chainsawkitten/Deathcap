#pragma once

#include "SuperComponent.hpp"

class PhysicsManager;

namespace Component {
    /// %Component that allows interacting with other physics components.
    class RigidBody : public SuperComponent {
        friend class ::PhysicsManager;

        public:
            /// Constructor.
            RigidBody() = default;

            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;
    };
}
