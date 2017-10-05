#pragma once

#include "Editors/ModelEditor.hpp"
#include "Editors/SceneEditor.hpp"
#include "Editors/SoundEditor.hpp"
#include "Editors/ScriptEditor.hpp"
#include "Editors/TextureEditor.hpp"
#include "SavePromptWindow.hpp"
#include "FolderNameWindow.hpp"
#include "../Resources.hpp"

namespace GUI {
    /// Displays all the hymn's resources.
    class ResourceView {
        public:
            /// Constructor.
            ResourceView();

            /// Show the resource list.
            void Show();
            
            /// Get whether the resource list is visible.
            /**
             * @return Whether the resource list is visible.
             */
            bool IsVisible() const;
            
            /// Set whether the resource list should be visible.
            /**
             * @param visible Whether the resource list should be visible.
             */
            void SetVisible(bool visible);
            
            /// Hide all editors.
            /**
             * Needs to be called before playing the game or old editors with stale pointers could be shown when returning to the editor.
             */
            void HideEditors();
            
            /// Save the currently active scene.
            void SaveScene() const;
            
            /// Reset which scene is open.
            void ResetScene();

            /// Return Scene Editor
            /**
             * @return The scene editor
             */
            SceneEditor& GetScene();
            
        private:
            void ShowResourceFolder(ResourceList::ResourceFolder& folder, const std::string& path);
            bool ShowResource(ResourceList::Resource& resource, const std::string& path);
            void FileNameWindowClosed(const std::string& name);
            
            bool visible = false;
            
            ScriptEditor scriptEditor;
            SceneEditor sceneEditor;
            ModelEditor modelEditor;
            TextureEditor textureEditor;
            SoundEditor soundEditor;
            
            SavePromptWindow savePromptWindow;
            FolderNameWindow folderNameWindow;
            
            bool changeScene = false;
            std::string resourcePath = "";
            std::string* scene = nullptr;
            ResourceList::ResourceFolder* parentFolder;
            
            static const int splitterSize = 2;
            int resourceHeight = 250;
            bool resourceResize = false;
            
            int sceneWidth = 250;
            bool sceneResize = false;
            
            int editorWidth = 250;
            bool editorResize = false;
            
            bool scriptPressed;
            bool texturePressed;
            bool modelPressed;
            bool soundPressed;
    };
}
