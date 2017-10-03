#pragma once

#include "SuperComponent.hpp"

namespace Physics {
    class Shape;
}

namespace Component {
    /// Physical shape that can be used in conjunction with rigid bodies or to
    /// act as a trigger volume.
    class Shape : public SuperComponent {
        friend class PhysicsManager;

        public:
            /// Create new shape.
            Shape() = default;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;

            /// Get the shape this component represents.
            /**
             * @return The %Shape.
             */
            ::Physics::Shape& GetShape();
            const ::Physics::Shape& GetShape() const;

        private:
            /// Set the shape that this component will represent.
            /**
             * @param shape The new shape.
             */
            void SetShape(::Physics::Shape* shape);

        private:
            ::Physics::Shape* shape = nullptr;
    };
}
