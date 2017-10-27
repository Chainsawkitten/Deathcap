#pragma once

namespace GUI {
    class LogView {
        public:
        void Show();

        bool IsVisible();
        
        private:
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