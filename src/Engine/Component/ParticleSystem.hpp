#include "SuperComponent.hpp"
#include <glm/glm.hpp>
#include <Video/ParticleSystemRenderer.hpp>
#include "../linking.hpp"

namespace Component {
    /// Emitter that emits particles.
    class ParticleSystemComponent : public SuperComponent {
        public:
        /// Defines a kind of particle.

        /// Create new particle emitter.
        ENGINE_API ParticleSystemComponent();

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
