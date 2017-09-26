#pragma once

#include <glm/glm.hpp>
#include "IRigidBody.hpp"

class btRigidBody;

namespace Physics {
    class Shape;

    class RigidBody : public IRigidBody {
        public:
            /// Constructor.
            RigidBody(Physics::Shape* shape, float mass);

            /// Destructor.
            ~RigidBody();

            /// Set the mass of a rigid body.
            /**
            * @param mass Mass in kilograms.
            */
            virtual void Mass(float mass) override;

            /// Get the position of a rigid body.
            /**
            * @return Position of the rigid body.
            */
            virtual glm::vec3 Position() override;

            /// Set the position of a rigid body.
            /**
            * @param pos The new position.
            */
            virtual void Position(glm::vec3 const& pos) override;

            btRigidBody* GetRigidBody();

            /// Get the shape associated with the rigid body.
            /**
             * @return The %Shape of the rigid body.
             */
            Physics::Shape& GetShape();

        private:
            RigidBody(RigidBody& other) = delete;
            void operator=(RigidBody& rhs) = delete;

        private:
            btRigidBody* rigidBody = nullptr;
            Physics::Shape* shape = nullptr;
    };

}
