#pragma once

#include <imgui.h>

/// Generic node editor.
class NodeEditor {
    public:
        /// Create new animation controller editor editor.
        NodeEditor();
    
        /// Destructor.
        ~NodeEditor();
    
        /// Show the editor.
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
    
    protected:
        virtual void DisplayNode(unsigned int index) = 0;

    private:
        void Save();
        void DrawGrid();
        void DrawActions();
        void DrawTransitions();
        void DrawConnections();
    
        bool visible = false;
    
        bool isDragingConnection = false;
        int hoveredNodeIndex = -1;
        int dragNodeIndex = -1;
    
        bool inited = false;
        ImVec2 scrolling = ImVec2(0.0f, 0.0f);
        bool show_grid = true;
        int node_selected = -1;
        bool open_context_menu = false;
        int node_hovered_in_list = -1;
        int node_hovered_in_scene = -1;
    
        float NODE_SLOT_RADIUS = 6.0f;
        ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
};