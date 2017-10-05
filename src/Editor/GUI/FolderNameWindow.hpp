#pragma once

namespace GUI {
    /// A window that asks the user for the name of the folder to create.
    class FolderNameWindow {
        public:
            /// Show the window and let the user enter a name.
            void Show();
            
            /// Get whether the window is visible.
            /**
             * @return Whether the window is visible.
             */
            bool IsVisible() const;
            
            /// Set whether the window should be visible.
            /**
             * @param visible Whether the window should be visible.
             */
            void SetVisible(bool visible);
            
            /// Reset the entered name.
            void ResetName();
            
            /// Get the entered name.
            /**
             * @return The name the user entered.
             */
            const char* GetName() const;
            
        private:
            bool visible = false;
            char name[100] = { '\0' };
    };
}
