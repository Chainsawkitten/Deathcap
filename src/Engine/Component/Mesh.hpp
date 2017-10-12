#pragma once

#include "SuperComponent.hpp"

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
            Mesh();
            
            /// Destructor.
            ~Mesh() override;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            Json::Value Save() const override;

            /// Geometry data.
            /**
             * Default: nullptr
             */
            Video::Geometry::Geometry3D* geometry = nullptr;

            /// Get mesh selection.
            /**
            * @return Bool Is this selected or not.
            */
            bool GetSelected() const;

            /// Set mesh selection.
            /**
            * @param Value If selected or not.
            */
            void SetSelected(bool value);

    private:        
        bool isSelected;

    };
}
