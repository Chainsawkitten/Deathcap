#pragma once

#include <string>
#include "../FileSelector.hpp"

namespace Audio {
    class AudioMaterial;
}

namespace GUI {
    /// Used to edit an audio material.
    class AudioMaterialEditor {
        public:
            /// Constructor.
            AudioMaterialEditor();

            /// Show the editor.
            void Show();
            
            /// Get the audio material being edited.
            /**
             * @return The audio material being edited.
             */
            const Audio::AudioMaterial* GetAudioMaterial() const;
            
            /// Set the audio material to edit.
            /**
             * @param audioMaterial Audio material to edit.
             */
            void SetAudioMaterial(Audio::AudioMaterial* audioMaterial);
            
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
            
        private:    
            Audio::AudioMaterial* audioMaterial = nullptr;
            bool visible = false;
            
            FileSelector fileSelector;
            
            char name[128];
    };
}
