#pragma once
#include <sstream>
#include <imgui.h>

namespace GUI {
    class LogView {
        public:
        /// Constructor.
        LogView();

        /// Destructor.
        ~LogView();

        /// Shows the log.
        void Show();

        /// If the log is visible or not.
        /**
         * @return Whether the log is visible or not.
         */
        bool IsVisible();
        
        private:
        std::stringstream defaultStringstream;
        std::stringstream infoStringstream;
        std::stringstream warningStringstream;
        std::stringstream errorStringstream;

        ImGuiTextBuffer textBuffer;
        ImGuiTextFilter textFilter;
        ImVector<int> lineOffsets;

        bool visible = true;

        static const int splitterSize = 2;
        int logHeight = 250;
        bool logResize = false;

        int sceneWidth = 250;
        bool sceneResize = false;

        int editorWidth = 250;
        bool editorResize = false;
    };
}
