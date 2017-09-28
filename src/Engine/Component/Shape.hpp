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
            /// Create new shape.
            Shape();
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;
            
        private:
            btCollisionShape* shape = nullptr;
    };
}
