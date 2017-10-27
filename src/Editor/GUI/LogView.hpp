#pragma once
#include <sstream>
#include <imgui.h>

namespace GUI {
    class LogView {
        public:
        LogView();

        ~LogView();

        void Show();

        bool IsVisible();
        
        private:
        class LogStream : public std::ostream {
        public:
            LogStream(std::stringbuf* stringBuffer);
        };

        LogStream* defaultStream;
        LogStream* infoStream;
        LogStream* warningStream;
        LogStream* errorStream;

        std::stringbuf* defaultBuffer;
        std::stringbuf* infoBuffer;
        std::stringbuf* warningBuffer;
        std::stringbuf* errorBuffer;

        ImGuiTextBuffer textBuffer;
        ImGuiTextFilter textFilter;

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