#pragma once

#include <json/json.h>
#include "../linking.hpp"

class Entity;

namespace Component {
    /// %Component which all components inherit.
    class SuperComponent {
        public:

            /// Create new %SuperComponent.
            ENGINE_EXPORT SuperComponent();
            
            /// Destructor.
            ENGINE_EXPORT virtual ~SuperComponent();
            
            /// Kill the component, will be removed at the end of the frame.
            ENGINE_EXPORT void Kill();
            
            /// Get whether component has been killed.
            /**
             * @return Whether component has been killed.
             */
            ENGINE_EXPORT bool IsKilled() const;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_EXPORT virtual Json::Value Save() const;
            
            /// Pointer to which entity this component corresponds.
            Entity* entity = nullptr;
            
        private:
            bool killed = false;
    };
}
