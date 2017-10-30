#pragma once
#include <cstdint>

namespace Component {
    class Trigger;
}

namespace GUI {
    /// Editor controls for the trigger component. Displays a modal window with
    /// settings for a given trigger component.
    class TriggerEditor {
        public:
            /// Open the trigger editor.
            void Open();

            /// Display the editor.
            /**
             * @param comp Trigger component to edit.
             */
            void Show(Component::Trigger& comp);

        private:
            uint32_t selectedTab = 0;
    };
}
