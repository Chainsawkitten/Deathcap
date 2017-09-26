#pragma once

#include "IShapeEditor.hpp"

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
            virtual void Show() override;
    };

}
