#pragma once

namespace GUI {

    /// Interface to editors of physics shapes.
    class IShapeEditor {
        public:
            /// Destructor.
            virtual ~IShapeEditor() {}

            /// Get a string with the kind of shape that's being edited. This
            /// is intended to be used as a label in the GUI.
            /**
             * @return The type of shape as a string.
             */
            virtual const char* Label() const = 0;

            /// Displays the editor.
            virtual void Show() = 0;
    };

}
