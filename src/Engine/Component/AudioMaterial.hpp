#pragma once

#include "SuperComponent.hpp"
#include "../linking.hpp"

namespace Component {
    /// %Component providing an audio material to Entity.
    class AudioMaterial : public SuperComponent {
        public:
            /// Create new audio material.
            ENGINE_API AudioMaterial();
            
            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;

    };
}
