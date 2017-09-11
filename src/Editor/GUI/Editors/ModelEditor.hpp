#pragma once

#include <string>
#include "../FileSelector.hpp"
#include "Util/AssetConverter.hpp"

namespace Geometry {
    class Model;
}

namespace GUI {
    /// Used to edit a model.
    class ModelEditor {
        public:
            /// Show the editor.
            void Show();
            
            /// Get the model being edited.
            /**
             * @return The model being edited.
             */
            const Geometry::Model* GetModel() const;
            
            /// Set the model to edit.
            /**
             * @param model Model to edit.
             */
            void SetModel(Geometry::Model* model);
            
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
            void FileSelected(const std::string& file);
            
            Geometry::Model* model = nullptr;
            bool visible = false;
            
            FileSelector fileSelector;
            
            char name[128] = "";

            bool hasSourceFile = false;
            std::string source;
            std::string destination;

			bool triangulate = true;
			bool importNormals = true;
			bool importTangents = true;
    };
}
