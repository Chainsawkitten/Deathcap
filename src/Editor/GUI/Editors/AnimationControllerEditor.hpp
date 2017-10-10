#pragma once

#include "NodeEditor.hpp"

namespace Animation {
    class AnimationClip;
    class AnimationController;
    class Skeleton;
}

class AnimationControllerEditor : public NodeEditor {
    public:
        /// Create new animation controller editor editor.
        AnimationControllerEditor();

        /// Destructor.
        ~AnimationControllerEditor();

    protected:
        // Inherited via NodeEditor
        virtual void DisplayNode(unsigned int index) override;

};