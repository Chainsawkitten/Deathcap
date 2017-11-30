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
            ENGINE_API Mesh();
            
            /// Destructor.
            ENGINE_API ~Mesh() override;
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;

            /// Geometry data.
            /**
             * Default: nullptr
             */
            Video::Geometry::Geometry3D* geometry = nullptr;
    };
}
