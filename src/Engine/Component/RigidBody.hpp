#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "SuperComponent.hpp"
#include "../linking.hpp"

class btGhostObject;
class btRigidBody;
class PhysicsManager;

namespace Physics {
    class Shape;
    class Trigger;
}

namespace Component {
    /// %Component that allows interacting with other physics components.
    class RigidBody : public SuperComponent {
        friend class ::PhysicsManager;
        friend class ::Physics::Trigger;

        public:
            /// Constructor.
            ENGINE_API RigidBody() = default;

            /// Destructor.
            ENGINE_API ~RigidBody();

            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;

            /// Return a value indicating whether the rigid body is kinematic
            /// or dynamic. In the former case the transform is determined by
            /// that of its entity. In the latter case, Bullet calculates it.
            /**
             * @return True if kinematic, false if dynamic.
             */
            ENGINE_API bool IsKinematic() const;

            /// Get the friction coefficient of the rigid body. Note that this
            /// does not necessarily match the real world as objects don't have
            /// one single value for friction.
            /**
             * @return Friction value.
             */
            ENGINE_API float GetFriction() const;

            /// Get the rolling friction coefficient of the rigid body.
            /**
             * @return Rolling friction value.
             */
            ENGINE_API float GetRollingFriction() const;

            /// Get the spinning friction coefficient of the rigid body.
            /**
             * @return Spinning friction value.
             */
            ENGINE_API float GetSpinningFriction() const;

            /// Get the coefficient of restitution of the rigid body.
            /**
             * @return Coefficient of restitution.
             */
            ENGINE_API float GetRestitution() const;

            /// Get the linear damping of the rigid body.
            /**
             * @return Linear damping.
             */
            ENGINE_API float GetLinearDamping() const;

            /// Get the angular damping of the rigid body.
            /**
             * @return Angular damping.
             */
            ENGINE_API float GetAngularDamping() const;

        private:
            // Get the underlying Bullet rigid body. If none has been set,
            // nullptr is returned.
            btRigidBody* GetBulletRigidBody();

            // Creates the underlying Bullet rigid body. Mass is provided in
            // units of kilograms.
            void NewBulletRigidBody(float mass);

            // Destroy resources completely.
            void Destroy();

            void SetCollisionShape(std::shared_ptr<Physics::Shape> shape);

            // Get the position of a rigid body.
            glm::vec3 GetPosition() const;

            // Set the position of a rigid body.
            void SetPosition(const glm::vec3& pos);

            // Get the orientation of a rigid body.
            glm::quat GetOrientation() const;

            // Set the orientation of a rigid body.
            void SetOrientation(const glm::quat& rotation);

            // Get the mass in kilograms of a rigid body.
            float GetMass();

            // Set the mass in kilograms of a rigid body.
            void SetMass(float mass);

            void SetFriction(float friction);
            void SetRollingFriction(float friction);
            void SetSpinningFriction(float friction);

            void SetRestitution(float cor);

            void SetLinearDamping(float damping);
            void SetAngularDamping(float damping);

            void MakeKinematic();
            void MakeDynamic();
            void SetGhost(bool ghost);

            // Get/set whether a dynamic rigid body should synchronize its
            // transform against the owning entity during the next simulation.
            bool GetForceTransformSync() const;
            void SetForceTransformSync(bool sync);

            // Get/set whether a kinematic rigid body should halt its movement
            // during the next simulation frame.
            bool GetHaltMovement() const;
            void SetHaltMovement(bool halt);

            float mass = 1.0f;
            btRigidBody* rigidBody = nullptr;
            btGhostObject* ghostObject = nullptr;
            bool kinematic = false;
            bool forceTransformSync = true; // For first frame
            bool haltMovement = true; // True for first frame
            float friction = 0.0f;
            float rollingFriction = 0.0f;
            float spinningFriction = 0.0f;
            float restitution = 0.0f;
            float linearDamping = 0.0f;
            float angularDamping = 0.0f;
            bool ghost = false;
            std::shared_ptr<Physics::Shape> shape;
    };
}
