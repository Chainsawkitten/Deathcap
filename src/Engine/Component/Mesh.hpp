#pragma once

#include "SuperComponent.hpp"
#include "../linking.hpp"

namespace Video {
    namespace Geometry {
        class Geometry3D;
    }
}

namespace Component {
    /// %Component providing geometry to an entity.
    class Mesh : public SuperComponent {
        public:
            /// Create new mesh.
            ENGINE_EXPORT Mesh();
            
            /// Destructor.
            ENGINE_EXPORT ~Mesh() override;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_EXPORT Json::Value Save() const override;

            /// Geometry data.
            /**
             * Default: nullptr
             */
            Video::Geometry::Geometry3D* geometry = nullptr;

            /// Get mesh selection.
            /**
             * @return Is this selected or not.
             */
            ENGINE_EXPORT bool GetSelected() const;

            /// Set mesh selection.
            /**
             * @param value If selected or not.
             */
            ENGINE_EXPORT void SetSelected(bool value);

    private:        
        bool isSelected;

    };
}
