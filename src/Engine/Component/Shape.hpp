#pragma once

#include "SuperComponent.hpp"

class btCollisionShape;
class Entity;

namespace Component {

    /// Physical shape that can be used in conjunction with rigid bodies or to
    /// act as a trigger volume.
    class Shape : public SuperComponent {
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

        public:
            /// Constructor.
            /**
             * @param entity Pointer to the entity this component belongs.
             */
            Shape(Entity* entity);

        private:
            btCollisionShape* shape = nullptr;
    };

}
