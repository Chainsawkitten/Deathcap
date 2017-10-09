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

            /// Highlight selection
            /**
            * @return True if selected else false.
            */
            bool GetSelected() const;

            /// Highlight selection
            /**
            * @param Set true if mesh is selected, else false.
            */
            void SetSelected(bool value);

    private:
        /// If mesh is selected in the editor.
        bool isSelected;

    };
}
