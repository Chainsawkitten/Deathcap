#pragma once

namespace Animation {
    class AnimationClip;
    class AnimationController;
    class Skeleton;
}

class AnimationControllerEditor {
    public:
        /// Create new animation controller editor editor.
        AnimationControllerEditor();

        /// Destructor.
        ~AnimationControllerEditor();

        /// Show the editor.
        void Show();

        /// Set the entity to edit.
        /**
        * @param entity The entity to edit.
        */
        void SetAnimationController(Animation::AnimationController* animationController);

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
        void Save();

        bool visible = false;
        Animation::AnimationController* animationController;
};