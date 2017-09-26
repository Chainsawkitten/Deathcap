#pragma once

#include <glm/glm.hpp>

namespace Physics {

    /// Interface to a rigid body.
    class IRigidBody {
        public:
            /// Set the mass of a rigid body.
            /**
             * @param mass Mass in kilograms.
             */
            virtual void Mass(float mass) = 0;

            /// Get the position of a rigid body.
            /**
             * @return Position of the rigid body.
             */
            virtual glm::vec3 Position() = 0;

            /// Set the position of a rigid body.
            /**
             * @param pos The new position.
             */
            virtual void Position(glm::vec3 const& pos) = 0;
    };

}
