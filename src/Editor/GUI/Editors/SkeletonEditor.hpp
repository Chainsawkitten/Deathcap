#pragma once

#include "../FileSelector.hpp"
#include "Util/AssetConverter.hpp"
#include "../../Resources.hpp"

namespace Animation {
    class Skeleton;
}

namespace GUI {
    /// Editor for skeleton.
    class SkeletonEditor {
        public:
            /// Constructor.
            SkeletonEditor();

            /// Show the editor.
            void Show();

            /// Set the skeleton to edit
            /**
             * @param folder Resource folder containing the model.
             * @param model Model to edit.
             */
            void SetSkeleton(ResourceList::ResourceFolder* folder, Animation::Skeleton skeleton);
            
            /// Get whether the window is visable.
            /**
             * @return True if visable.
             */
            bool IsVisable() const;

            /// Set whether the window should be visable.
            /**
             * @param visable Whether the window should be visible.
             */
            void SetVisable(bool visable);

        private:
            void FileSelected(const std::string& file);

    };
}