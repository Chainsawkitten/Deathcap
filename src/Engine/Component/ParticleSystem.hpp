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

        /// Save the component.
        /**
        * @return JSON value to be stored on disk.
        */
        ENGINE_API Json::Value Save() const override;

        /// Particletype
        Video::ParticleSystemRenderer::EmitterSettings particleType;

        /// Size of the emitter (area of effect).
        glm::vec3 size;

        /// Average time for emitting particle (in seconds).
        float averageEmitTime = 0.04f;

        /// Time variance for emitting particle (in seconds).
        float emitTimeVariance = 0.01f;

        /// Time to next emit (random value between min and max),
        float timeToNext = 0.1f;

        /// Whether to preview the emitter in the editor.
        bool preview = false;

    };
}
