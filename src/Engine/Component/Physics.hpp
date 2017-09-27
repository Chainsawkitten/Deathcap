#pragma once

#include "SuperComponent.hpp"
#include <glm/glm.hpp>

class Entity;

namespace Physics {
    class RigidBody;
}

namespace Component {
    /// %Component allows interacting with other physics components.
    class Physics : public SuperComponent {
        friend class PhysicsManager;

        public:
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;

            /// Load component from JSON node.
            /**
             * @param node JSON node to load from.
             */
            void Load(const Json::Value& node) override;

            /// Velocity (in meter/second).
            glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);

            /// Max velocity (in meter/second).
            float maxVelocity = 20.f;

            /// Angular velocity (in Hz).
            glm::vec3 angularVelocity = glm::vec3(0.f, 0.f, 0.f);

            /// Max angular velocity (in Hz).
            float maxAngularVelocity = 2.f;

            /// Acceleration (in meter/second^2).
            glm::vec3 acceleration = glm::vec3(0.f, 0.f, 0.f);

            /// Angular acceleration (in Hz/second).
            glm::vec3 angularAcceleration = glm::vec3(0.f, 0.f, 0.f);

            /// Friction factor. Describes the percentage of the velocity that is lost each second. 
            float velocityDragFactor = 1.f;

            /// Angular drag factor. Describes the percentage of the rotation that is lost each second. 
            float angularDragFactor = 1.f;

            /// Gravity factor. 
            float gravityFactor = 0.f;

            /// Moment of inertia. Describes how hard it's to rotate an object.
            /**
             * https://en.wikipedia.org/wiki/List_of_moments_of_inertia
             */
            glm::vec3 momentOfInertia = glm::vec3(1.f, 1.f, 1.f);

            /// Create new physics component.
            /**
             * @param entity Pointer to which entity this component corresponds.
             */
            Physics(Entity* entity);
            
        private:
            ::Physics::RigidBody* rigidBody = nullptr;
    };
}
