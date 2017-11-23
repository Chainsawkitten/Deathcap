#pragma once

#include <Engine/Component/SuperComponent.hpp>
#include <glm/glm.hpp>
#include <Video/ParticleSystemRenderer.hpp>
#include "../linking.hpp"

namespace Component {
    /// Emitter that emits particles.
    class ParticleSystemComponent : public SuperComponent {
        public:
            /// Create new particle emitter.
            ENGINE_API ParticleSystemComponent();

            /// Destructor.
            ENGINE_API ~ParticleSystemComponent();

            /// Save the component.
            /**
             * @return JSON value to be stored on disk.
             */
            ENGINE_API Json::Value Save() const override;

            /// Particletype
            Video::ParticleSystemRenderer::EmitterSettings particleType;
    };
} // namespace Component
