#pragma once

#include "SuperComponent.hpp"
#include "../Audio/AudioMaterial.hpp"
#include "../linking.hpp"

namespace Audio {
    class AudioMaterial;
}

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

            /// Audio material contaning information about how sound should interact with the material.
            Audio::AudioMaterial* material = nullptr;
    };
}
