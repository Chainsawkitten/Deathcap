#pragma once

#include "../Resources.hpp"

namespace GUI {
    /// Used to select a resource from the resource list.
    class ResourceSelector {
        public:
            /// Show the resource selector and let the user select a resource.
            /**
             * @param type The type of resource to select.
             */
            bool Show(ResourceList::Resource::Type type);
            
        private:
            bool ShowHelper(ResourceList::Resource::Type type, const ResourceList::ResourceFolder& folder);
    };
}
