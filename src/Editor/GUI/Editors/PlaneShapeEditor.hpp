#pragma once

#include "IShapeEditor.hpp"

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
            virtual void Show() override;
    };

}
