#include "ResourceSelector.hpp"

#include <imgui.h>

using namespace GUI;

std::string ResourceSelector::SelectedResource::GetPath() const {
    return path + "/" + resource->GetName();
}

bool ResourceSelector::Show(ResourceList::Resource::Type type) {
    return ShowHelper(type, Resources().resourceFolder, Resources().resourceFolder.name);
}

const ResourceSelector::SelectedResource& ResourceSelector::GetSelectedResource() const {
    return selectedResource;
}

bool ResourceSelector::ShowHelper(ResourceList::Resource::Type type, const ResourceList::ResourceFolder& folder, const std::string& path) {
    // Show subfolders.
    for (const ResourceList::ResourceFolder& subfolder : folder.subfolders) {
        if (ImGui::TreeNode(subfolder.name.c_str())) {
            bool subfolderSelected = ShowHelper(type, subfolder, path + "/" + subfolder.name);
            ImGui::TreePop();
            if (subfolderSelected)
                return true;
        }
    }

    // Show resources.
    for (const ResourceList::Resource& resource : folder.resources) {
        if (resource.type == type) {
            if (ImGui::Selectable(resource.GetName().c_str())) {
                selectedResource.resource = &resource;
                selectedResource.path = path;

                return true;
            }
        }
    }

    return false;
}
