#include "ResourceSelector.hpp"

#include <imgui.h>

using namespace GUI;

bool ResourceSelector::Show(ResourceList::Resource::Type type) {
    return ShowHelper(type, Resources().resourceFolder);
}

bool ResourceSelector::ShowHelper(ResourceList::Resource::Type type, const ResourceList::ResourceFolder& folder) {
    // Show subfolders.
    for (const ResourceList::ResourceFolder& subfolder : folder.subfolders) {
        if (ImGui::TreeNode(subfolder.name.c_str())) {
            bool subfolderSelected = ShowHelper(type, subfolder);
            ImGui::TreePop();
            if (subfolderSelected)
                return true;
        }
    }
    
    // Show resources.
    for (const ResourceList::Resource& resource : folder.resources) {
        if (resource.type == type) {
            if (ImGui::Selectable(resource.GetName().c_str())) {
                /// @todo Set selected resource.
                
                return true;
            }
        }
    }
    
    return false;
}
