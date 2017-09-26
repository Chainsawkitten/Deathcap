#pragma once

#include "IShapeEditor.hpp"

namespace Component {
    class Physics;
}

namespace GUI {

    /// Plane shape editor for physics components.
    class PlaneShapeEditor : public IShapeEditor {
        public:
            /// Plane type label.
            /**
             * @return "Plane" string.
             */
            virtual const char* Label() const override { return "Plane"; }

            /// Show controls for editing a plane shape.
            /**
             * @param comp The physics component to edit.
             */
            virtual void Show(Component::Physics* comp) override;

            /// Set a sphere shape on the given physics component.
            /**
             * @param comp The physics component on which to set shape.
             */
            virtual void Apply(Component::Physics* comp) override;
    };

}
