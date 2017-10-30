#pragma once

#include "../FileSelector.hpp"
#include "Util/AssetConverter.hpp"
#include "../../Resources.hpp"

namespace Animation {
    class AnimationClip;
}

namespace GUI {
    class AnimationClipEditor {
        public:
            /// Constructor.
            AnimationClipEditor();

            /// Show the editor.
            void Show();

            /// Returns the active animation clip.
            /**
             * @return The active animation clip. 
             */
            Animation::AnimationClip* GetAnimationClip();

            /// Set the animation clip to edit
            /**
             * @param folder Resource folder containing the animation clip.
             * @param model Animation clip to edit.
             */
            void SetAnimationClip(Animation::AnimationClip* animationClip);

            /// Get whether the window is visible.
            /**
             * @return True if visible.
             */
            bool IsVisible() const;

            /// Set whether the window should be visable.
            /**
             * @param visable Whether the window should be visible.
             */
            void SetVisible(bool visible);

        private:
            void FileSelected(const std::string& file);

            ResourceList::ResourceFolder* folder = nullptr;
            Animation::AnimationClip* animationClip = nullptr;
            bool visible = false;

            FileSelector fileSelector;
    };
}
