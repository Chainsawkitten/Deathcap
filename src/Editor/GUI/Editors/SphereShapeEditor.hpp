#pragma once

#include "IShapeEditor.hpp"

namespace Component {
    class Physics;
}

namespace GUI {

    /// Sphere shape editor for physics components.
    class SphereShapeEditor : public IShapeEditor {
        public:
            /// Sphere type label.
            /**
             * @return "Sphere" string.
             */
            virtual const char* Label() const override { return "Sphere"; }

            /// Show controls for editing a sphere shape.
            /**
             * @param comp The physics component to edit.
             */
            virtual void Show(Component::Physics* comp) override;

            /// Set a plane shape on the given physics component.
            /**
             * @param comp The physics component on which to set shape.
             */
            virtual void Apply(Component::Physics* comp) override;
    };

}
